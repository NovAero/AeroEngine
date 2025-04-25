#include "AEPCH.h"
#include "CommandAllocatorPool.h"

namespace AE::Graphics {
	CommandAllocatorPool::CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type) :
		m_commandListType(type),
		m_device(nullptr)
	{
	}

	CommandAllocatorPool::~CommandAllocatorPool()
	{
		Shutdown();
	}

	VOID CommandAllocatorPool::Create(ComPtr<ID3D12Device2> device)
	{
		m_device = device;
	}

	VOID CommandAllocatorPool::Shutdown()
	{
		for (size_t i = 0; i < m_allocatorPool.size(); ++i) {
			m_allocatorPool[i]->Release();
		}

		m_allocatorPool.clear();
	}

	ComPtr<ID3D12CommandAllocator> CommandAllocatorPool::RequestAllocator(UINT64 completedFenceValue)
	{
		std::lock_guard<std::mutex> lockGuard(m_allocatorMutex);

		ComPtr<ID3D12CommandAllocator> allocator = nullptr;

		if (!m_readyAllocators.empty())
		{
			std::pair<UINT64, ComPtr<ID3D12CommandAllocator>>& allocatorPair = m_readyAllocators.front();

			if (allocatorPair.first <= completedFenceValue)
			{
				allocator = allocatorPair.second.Get();
				ASSERT_SUCCEEDED(allocator->Reset());
				m_readyAllocators.pop();
			}
		}

		// If no allocators were ready to be reused, create a new one
		if (allocator == nullptr)
		{
			ASSERT_SUCCEEDED(m_device->CreateCommandAllocator(m_commandListType, IID_PPV_ARGS(&allocator)));
			WCHAR allocatorName[32];
			swprintf(allocatorName, 32, L"CommandAllocator %zu", m_allocatorPool.size());
			allocator->SetName(allocatorName);
			m_allocatorPool.push_back(allocator);
		}

		return allocator;
	}

	VOID CommandAllocatorPool::DiscardAllocator(UINT64 fenceValue, ComPtr<ID3D12CommandAllocator> allocator)
	{
		std::lock_guard<std::mutex> LockGuard(m_allocatorMutex);

		// That fence value indicates we are free to reset the allocator
		m_readyAllocators.push(std::make_pair(fenceValue, allocator));
	}
}