#pragma once

#include "DescriptorHeap.h"
#include "RootSigniature.h"
#include <vector>
#include <queue>

namespace AE::Graphics {

	using namespace Microsoft::WRL;

	extern ComPtr<ID3D12Device2> g_device;

	class DynamicDescriptorHeap {
	public:

		DynamicDescriptorHeap(CommandContext& owningContext, D3D12_DESCRIPTOR_HEAP_TYPE heapType);
		~DynamicDescriptorHeap();

		static void DestroyAll(void) {
			
		}


	private:
		
		//static members
		static const UINT32													sm_NumDescriptorsPerHeap = 1024;
		static std::mutex													sm_Mutex;
		static std::vector<ComPtr<ID3D12DescriptorHeap>>					sm_DescriptorHeapPool[2];
		static std::queue<std::pair<UINT64,ComPtr<ID3D12DescriptorHeap>>>	sm_RetiredDescriptorHeaps[2];
		static std::queue<ComPtr<ID3D12DescriptorHeap>>						sm_AvailableDescriptorHeaps[2];


		//non static members
		CommandContext&														m_owningContext;
		ComPtr<ID3D12DescriptorHeap>										m_currentHeapPtr;
		const D3D12_DESCRIPTOR_HEAP_TYPE									m_DESCRIPTORTYPE;
		UINT32																m_descriptorSize;
		UINT32																m_currentOffset;
		DescriptorHandle													m_firstDescriptor;
		std::vector<ComPtr<ID3D12DescriptorHeap>>							m_retiredHeaps;
	};

}