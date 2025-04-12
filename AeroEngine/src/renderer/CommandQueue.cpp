#include "AEPCH.h"
#include "CommandQueue.h"
#include "DXHelper.h"

namespace AE::Renderer {

	CommandQueue::CommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
		: m_FenceValue(0), m_CommandListType(type), m_d3d12Device(device)
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		ThrowIfFailed(m_d3d12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_d3d12CommandQueue)));
		ThrowIfFailed(m_d3d12Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_d3d12Fence)));

		m_FenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(m_FenceEvent && "Failed to create fence event handle.");
	}

	CommandQueue::~CommandQueue()
	{
	}

	ComPtr<ID3D12GraphicsCommandList2> CommandQueue::GetCommandList()
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList2> commandList;

		if (!m_CommandAllocatorQueue.empty() && IsFenceComplete(m_CommandAllocatorQueue.front().fenceValue)) {
			commandAllocator = m_CommandAllocatorQueue.front().commandAllocator;
			m_CommandAllocatorQueue.pop();

			ThrowIfFailed(commandAllocator->Reset());
		}
		else {
			commandAllocator = CreatecommandAllocator();
		}

		if (m_CommandListQueue.empty()) {
			commandList = m_CommandListQueue.front();
			m_CommandListQueue.pop();

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

		m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
		m_CommandListQueue.push(commandList);

		commandAllocator->Release();

		return fenceValue;
	}

	ComPtr<ID3D12CommandAllocator> CommandQueue::CreatecommandAllocator()
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ThrowIfFailed(m_d3d12Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)));

		return commandAllocator;
	}

	ComPtr<ID3D12GraphicsCommandList2> CommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator> allocator)
	{
		ComPtr<ID3D12GraphicsCommandList2> commandList;
		ThrowIfFailed(m_d3d12Device->CreateCommandList(0, m_CommandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

		return commandList;
	}
}
