#include "AEPCH.h"
#include "CommandQueue.h"
#include "DXHelper.h"

namespace AE::Renderer {

	CommandQueue::CommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
		: m_fenceValue(0), m_commandListType(type), m_d3d12Device(device)
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		ThrowIfFailed(m_d3d12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_d3d12CommandQueue)));
		ThrowIfFailed(m_d3d12Device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_d3d12Fence)));

		m_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(m_fenceEvent && "Failed to create fence event handle.");
	}

	CommandQueue::~CommandQueue()
	{
	}

	ComPtr<ID3D12GraphicsCommandList2> CommandQueue::GetCommandList()
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList2> commandList;

		if (!m_commandAllocatorQueue.empty() && IsFenceComplete(m_commandAllocatorQueue.front().FenceValue)) {
			commandAllocator = m_commandAllocatorQueue.front().CommandAllocator;
			m_commandAllocatorQueue.pop();

			ThrowIfFailed(commandAllocator->Reset());
		}
		else {
			commandAllocator = CreatecommandAllocator();
		}

		if (m_commandListQueue.empty()) {
			commandList = m_commandListQueue.front();
			m_commandListQueue.pop();

			ThrowIfFailed(commandList->Reset(commandAllocator.Get(), nullptr));

		}
		else {
			commandList = CreateCommandList(commandAllocator);
		}

		//Associate command allocator with the command list so that is can be
		//Retrieved whe nthe command list is executed
		ThrowIfFailed(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));

		return commandList;
	}

	UINT64 CommandQueue::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList)
	{
		commandList->Close();

		ID3D12CommandAllocator* commandAllocator;
		UINT dataSize = sizeof(commandAllocator);
		ThrowIfFailed(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));
		
		ID3D12CommandList* const ppCommandLists[] = {
			commandList.Get()
		};

		m_d3d12CommandQueue->ExecuteCommandLists(1, ppCommandLists);
		UINT64 fenceValue = Signal();

		m_commandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
		m_commandListQueue.push(commandList);

		commandAllocator->Release();

		return fenceValue;
	}

	UINT64 CommandQueue::Signal()
	{
		return UINT64();
	}

	BOOL CommandQueue::IsFenceComplete(UINT64 fenceValue)
	{
		return 0;
	}

	VOID CommandQueue::WaitForFenceValue(UINT64 fenceValue)
	{
	}

	VOID CommandQueue::Flush()
	{
	}

	ComPtr<ID3D12CommandQueue> CommandQueue::GetD3D12CommandQueue() const
	{
		return ComPtr<ID3D12CommandQueue>();
	}

	ComPtr<ID3D12CommandAllocator> CommandQueue::CreatecommandAllocator()
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ThrowIfFailed(m_d3d12Device->CreateCommandAllocator(m_commandListType, IID_PPV_ARGS(&commandAllocator)));

		return commandAllocator;
	}

	ComPtr<ID3D12GraphicsCommandList2> CommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator> allocator)
	{
		ComPtr<ID3D12GraphicsCommandList2> commandList;
		ThrowIfFailed(m_d3d12Device->CreateCommandList(0, m_commandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

		return commandList;
	}
}
