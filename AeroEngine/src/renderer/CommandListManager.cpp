#include "AEPCH.h"
#include "CommandListManager.h"
#include "DXHelper.h"

namespace AE::Graphics {

	/*extern*/ CommandListManager g_CommandManager;

	CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type) :
		m_type(type),
		m_commandQueue(nullptr),
		m_fence(nullptr),
		m_nextFenceValue((UINT64)type << 56 | 1),
		m_lastCompletedFenceValue((UINT64)type << 56),
		m_allocatorPool(type)
	{
	}

	CommandQueue::~CommandQueue()
	{
		Shutdown();
	}

	VOID CommandQueue::Create(ComPtr<ID3D12Device2> device)
	{
		ASSERT(device != nullptr);
		ASSERT(!IsReady());
		ASSERT(m_allocatorPool.Size() == 0);

		D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
		QueueDesc.Type = m_type;
		QueueDesc.NodeMask = 1;
		device->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&m_commandQueue));
		m_commandQueue->SetName(L"CommandListManager::m_CommandQueue");

		ASSERT_SUCCEEDED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
		m_fence->SetName(L"CommandListManager::m_pFence");
		m_fence->Signal((UINT64)m_type << 56);

		m_fenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
		ASSERT(m_fenceEventHandle != NULL);

		m_allocatorPool.Create(device);

		ASSERT(IsReady());
	}

	VOID CommandQueue::Shutdown()
	{
		if (!IsReady()) return;

		m_allocatorPool.Shutdown();

		CloseHandle(m_fenceEventHandle);

		m_fence->Release();
		m_fence = nullptr;

		m_commandQueue->Release();
		m_commandQueue = nullptr;
	}

	UINT64 CommandQueue::IncrementFence(void)
	{
		std::lock_guard<std::mutex> LockGuard(m_fenceMutex);
		m_commandQueue->Signal(m_fence.Get(), m_nextFenceValue);
		return m_nextFenceValue++;
	}

	BOOL CommandQueue::IsFenceComplete(UINT64 fenceValue)
	{
		// Avoid querying the fence value by testing against the last one seen.
		// The max() is to protect against an unlikely race condition that could cause the last
		// completed fence value to regress.
		if (fenceValue > m_lastCompletedFenceValue)
			m_lastCompletedFenceValue = std::max(m_lastCompletedFenceValue, m_fence->GetCompletedValue());

		return fenceValue <= m_lastCompletedFenceValue;
	}

	VOID CommandQueue::StallForFence(UINT64 fenceValue)
	{
		CommandQueue& Producer = g_CommandManager.GetQueue((D3D12_COMMAND_LIST_TYPE)(fenceValue >> 56));
		m_commandQueue->Wait(Producer.m_fence.Get(), fenceValue);
	}

	VOID CommandQueue::StallForProducer(CommandQueue& producer)
	{
		ASSERT(producer.m_nextFenceValue > 0);
		m_commandQueue->Wait(producer.m_fence.Get(), producer.m_nextFenceValue - 1);
	}

	VOID CommandQueue::WaitForFence(UINT64 fenceValue)
	{
		if (IsFenceComplete(fenceValue))
			return;

		// TODO:  Think about how this might affect a multi-threaded situation.  Suppose thread A
		// wants to wait for fence 100, then thread B comes along and wants to wait for 99.  If
		// the fence can only have one event set on completion, then thread B has to wait for 
		// 100 before it knows 99 is ready.  Maybe insert sequential events?
		{
			std::lock_guard<std::mutex> LockGuard(m_eventMutex);

			m_fence->SetEventOnCompletion(fenceValue, m_fenceEventHandle);
			WaitForSingleObject(m_fenceEventHandle, INFINITE);
			m_lastCompletedFenceValue = fenceValue;
		}
	}


	UINT64 CommandQueue::ExecuteCommandList(ComPtr<ID3D12CommandList> list)
	{
		std::lock_guard<std::mutex> LockGuard(m_fenceMutex);

		ASSERT_SUCCEEDED(((ID3D12GraphicsCommandList*)list.Get())->Close());

		// Kickoff the command list
		m_commandQueue->ExecuteCommandLists(1, &list);

		// Signal the next fence value (with the GPU)
		m_commandQueue->Signal(m_fence.Get(), m_nextFenceValue);

		// And increment the fence value.  
		return m_nextFenceValue++;
	}

	ComPtr<ID3D12CommandAllocator> CommandQueue::RequestAllocator(void)
	{
		UINT64 CompletedFence = m_fence->GetCompletedValue();
		return m_allocatorPool.RequestAllocator(CompletedFence);
	}

	VOID CommandQueue::DiscardAllocator(UINT64 fenceValue, ComPtr<ID3D12CommandAllocator> allocator)
	{
		m_allocatorPool.DiscardAllocator(fenceValue, allocator);
	}



	CommandListManager::CommandListManager() :
		m_device(nullptr),
		m_graphicsQueue(D3D12_COMMAND_LIST_TYPE_DIRECT),
		m_computeQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE),
		m_copyQueue(D3D12_COMMAND_LIST_TYPE_COPY)
	{
	}

	CommandListManager::~CommandListManager()
	{
		Shutdown();
	}

	VOID CommandListManager::Create(ComPtr<ID3D12Device2> device)
	{
		ASSERT(device != nullptr);

		m_device = device;

		m_graphicsQueue.Create(device);
		m_computeQueue.Create(device);
		m_copyQueue.Create(device);
	}

	VOID CommandListManager::Shutdown()
	{
		m_graphicsQueue.Shutdown();
		m_computeQueue.Shutdown();
		m_copyQueue.Shutdown();
	}

	CommandQueue& CommandListManager::GetQueue(D3D12_COMMAND_LIST_TYPE type)
	{
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_COMPUTE: return m_computeQueue;
		case D3D12_COMMAND_LIST_TYPE_COPY: return m_copyQueue;
		default: return m_graphicsQueue;
		}
	}

	ComPtr<ID3D12CommandQueue> CommandListManager::GetCommandQueue()
	{
		return m_graphicsQueue.GetCommandQueue();
	}

	VOID CommandListManager::CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** list, ID3D12CommandAllocator** allocator)
	{
		ASSERT(type != D3D12_COMMAND_LIST_TYPE_BUNDLE, "Bundles are not yet supported");
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT: *allocator = m_graphicsQueue.RequestAllocator().Get(); break;
		case D3D12_COMMAND_LIST_TYPE_BUNDLE: break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE: *allocator = m_computeQueue.RequestAllocator().Get(); break;
		case D3D12_COMMAND_LIST_TYPE_COPY: *allocator = m_copyQueue.RequestAllocator().Get(); break;
		}

		ASSERT_SUCCEEDED(m_device->CreateCommandList(1, type, *allocator, nullptr, IID_PPV_ARGS(list)));
		(*list)->SetName(L"CommandList");
	}

	BOOL CommandListManager::IsFenceComplete(UINT64 fenceValue)
	{
		return GetQueue(D3D12_COMMAND_LIST_TYPE(fenceValue >> 56)).IsFenceComplete(fenceValue);
	}

	VOID CommandListManager::WaitForFence(UINT64 fenceValue)
	{
		CommandQueue& producer = g_CommandManager.GetQueue((D3D12_COMMAND_LIST_TYPE)(fenceValue >> 56));
		producer.WaitForFence(fenceValue);
	}

	VOID CommandListManager::IdleGPU(void)
	{
		m_graphicsQueue.WaitForIdle();
		m_computeQueue.WaitForIdle();
		m_copyQueue.WaitForIdle();
	}

}