#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <stdint.h>

using namespace Microsoft::WRL;

namespace AE::Graphics {
	class CommandAllocatorPool {
	public:
		CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type);
		~CommandAllocatorPool();

		VOID								Create(ComPtr<ID3D12Device2> device);
		VOID								Shutdown();
		
		ComPtr<ID3D12CommandAllocator>		RequestAllocator(UINT64 completedFenceValue);
		VOID								DiscardAllocator(UINT64 fenceValue, ComPtr<ID3D12CommandAllocator> allocator);


		inline size_t Size() { return m_allocatorPool.size(); }

	private:

		const D3D12_COMMAND_LIST_TYPE									m_commandListType;

		ComPtr<ID3D12Device2>											m_device;
		std::vector<ComPtr<ID3D12CommandAllocator>>						m_allocatorPool;
		std::queue <std::pair<UINT64, ComPtr<ID3D12CommandAllocator>>>	m_readyAllocators;
		std::mutex														m_allocatorMutex;

	};
}