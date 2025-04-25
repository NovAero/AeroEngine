#pragma once

#include <mutex>
#include <vector>
#include <string>
#include <queue>

namespace AE::Graphics {

	using namespace Microsoft::WRL;

	class DescriptorAllocator {
	public:

		DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type);

		D3D12_CPU_DESCRIPTOR_HANDLE Allocate(UINT32 count);

		static void DestroyAll(void);

	protected:
		
		static ComPtr<ID3D12DescriptorHeap> RequestNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE type);

		static const UINT32								 sm_NumDescriptorsPerHeap = 256;
		static std::mutex								 sm_AllocationMutex;
		static std::vector<ComPtr<ID3D12DescriptorHeap>> sm_DescriptorHeapPool;

		D3D12_DESCRIPTOR_HEAP_TYPE		m_type;
		ComPtr<ID3D12DescriptorHeap>	m_currentHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE		m_currentHandle;
		uint32_t						m_descriptorSize;
		uint32_t						m_remainingFreeHandles;
	};

	class DescriptorHandle {
	public:
		DescriptorHandle();
		DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);

		DescriptorHandle operator+ (INT offsetScaledByDescriptorSize) const;
		void operator+=(INT offsetScaledByDescriptorSize);

		const D3D12_CPU_DESCRIPTOR_HANDLE* operator&() const { return &m_cpuHandle; }
		operator D3D12_CPU_DESCRIPTOR_HANDLE() const { return m_cpuHandle;}
		operator D3D12_GPU_DESCRIPTOR_HANDLE() const { return m_gpuHandle;}

		size_t GetCpuPtr() const { return m_cpuHandle.ptr; }
		UINT64 GetGpuPtr() const { return m_gpuHandle.ptr; }
		bool IsNull() const { return m_cpuHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN; }
		bool IsShaderVisible() const { return m_gpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN; }

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE		m_cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE		m_gpuHandle;
	};

	class DescriptorHeap {
	public:

		DescriptorHeap(void) {}
		~DescriptorHeap(void) { Destroy(); }

	public:

		void						 Create(const WSTRING& debugHeapName, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT32 maxCount);
		void						 Destroy(void) { m_heap.Reset(); }
									 
		bool						 HasAvailableSpace(UINT32 count) const;
		DescriptorHandle			 Alloc(UINT32 count = 1);
									 
		DescriptorHandle			 operator[](UINT32 arrayIndex) const;
									 
		UINT32						 GetOffsetOfHandle(const DescriptorHandle& dHandle);
									 
		bool						 ValidateHandle(const DescriptorHandle& dHandle) const;

		ComPtr<ID3D12DescriptorHeap> GetHeapCPointer() const { return m_heap; }

		UINT32						 GetDescriptorSize(void) const { return m_descriptorSize; }

	private:

		ComPtr<ID3D12DescriptorHeap>	m_heap;
		D3D12_DESCRIPTOR_HEAP_DESC		m_heapDesc;
		UINT32							m_descriptorSize;
		UINT32							m_numFreeDescriptors;
		DescriptorHandle				m_firstHandle;
		DescriptorHandle				m_nextFreeHandle;
	};
}