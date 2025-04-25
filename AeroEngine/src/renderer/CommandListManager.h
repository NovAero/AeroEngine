#pragma once 

#include <vector>
#include <queue>
#include <mutex>
#include <stdint.h>
#include "CommandAllocatorPool.h"

using namespace Microsoft::WRL;

namespace AE::Graphics {
	class CommandQueue {
		friend class CommandListManager;
		friend class CommandContext;

	public:
		CommandQueue(D3D12_COMMAND_LIST_TYPE type);
		~CommandQueue();

		VOID								Create(ComPtr<ID3D12Device2> device);
		VOID								Shutdown();

		inline BOOL							IsReady() { return m_commandQueue != nullptr; }

		UINT64								IncrementFence(void);
		BOOL								IsFenceComplete(UINT64 fenceValue);
		VOID								StallForFence(UINT64 fenceValue);
		VOID								StallForProducer(CommandQueue& producer);
		VOID								WaitForFence(UINT64 fenceValue);
		VOID								WaitForIdle(void) { WaitForFence(IncrementFence()); }

		ComPtr<ID3D12CommandQueue>			GetCommandQueue() { return m_commandQueue; }

		UINT64								NextFenceValue() { return m_nextFenceValue; }

	private:

		UINT64								ExecuteCommandList(ComPtr<ID3D12CommandList> list);
		ComPtr<ID3D12CommandAllocator>		RequestAllocator(void);
		VOID								DiscardAllocator(UINT64 fenceValue, ComPtr<ID3D12CommandAllocator> allocator);


		ComPtr<ID3D12CommandQueue>						m_commandQueue;

		const D3D12_COMMAND_LIST_TYPE					m_type;

		CommandAllocatorPool							m_allocatorPool;
		std::mutex										m_fenceMutex;
		std::mutex										m_eventMutex;

		ComPtr<ID3D12Fence>								m_fence;
		UINT64											m_nextFenceValue;
		UINT64											m_lastCompletedFenceValue;
		HANDLE											m_fenceEventHandle;

	};

	class CommandListManager {
		friend class CommandContext;

	public:
		CommandListManager();
		~CommandListManager();

		VOID								Create(ComPtr<ID3D12Device2> device);
		VOID								Shutdown();

		CommandQueue&						GetQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
		ComPtr<ID3D12CommandQueue>			GetCommandQueue();

		VOID								CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** list, ID3D12CommandAllocator** allocator);

		BOOL								IsFenceComplete(UINT64 fenceValue);

		//CPU will wait for fence to reach a specified value
		VOID								WaitForFence(UINT64 fenceValue);

		//CPU will wait for all command queues to empty so that the GPU is idle
		VOID								IdleGPU(void);

		CommandQueue&						GraphicsQueue(void) { return m_graphicsQueue; }
		CommandQueue&						ComputeQueue(void) { return m_computeQueue; }
		CommandQueue&						CopyQueue(void) { return m_copyQueue; }

	private:

		ComPtr<ID3D12Device2> m_device;

		CommandQueue m_graphicsQueue;
		CommandQueue m_computeQueue;
		CommandQueue m_copyQueue;
	};
}