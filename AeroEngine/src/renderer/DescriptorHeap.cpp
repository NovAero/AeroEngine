#include "AEPCH.h"
#include "DescriptorHeap.h"
#include "GraphicsCore.h"
#include "CommandListManager.h"
#include "DXHelper.h"

namespace AE::Graphics {

    std::mutex DescriptorAllocator::sm_AllocationMutex;
    std::vector<ComPtr<ID3D12DescriptorHeap>> DescriptorAllocator::sm_DescriptorHeapPool;

    DescriptorAllocator::DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type)
        : m_type(type),
        m_currentHeap(nullptr),
        m_descriptorSize(0)
    {
        m_currentHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::Allocate(UINT32 count)
    {

        if (m_currentHeap == nullptr || m_remainingFreeHandles < count) {
            m_currentHeap = RequestNewHeap(m_type);
            m_currentHandle = m_currentHeap->GetCPUDescriptorHandleForHeapStart();
            m_remainingFreeHandles = sm_NumDescriptorsPerHeap;

            if (m_descriptorSize == 0) {
                m_descriptorSize = g_Device->GetDescriptorHandleIncrementSize(m_type);
            }
        }

        D3D12_CPU_DESCRIPTOR_HANDLE ret = m_currentHandle;
        m_currentHandle.ptr += count * m_descriptorSize;
        m_remainingFreeHandles -= count;
        return ret;
    }

    void DescriptorAllocator::DestroyAll(void)
    {
        sm_DescriptorHeapPool.clear();
    }

    ComPtr<ID3D12DescriptorHeap> DescriptorAllocator::RequestNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE type)
    {
        std::lock_guard<std::mutex> LockGuard(sm_AllocationMutex);

        D3D12_DESCRIPTOR_HEAP_DESC desc;
        desc.Type = type;
        desc.NumDescriptors = sm_NumDescriptorsPerHeap;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 1;

        ComPtr<ID3D12DescriptorHeap> heap;

        ASSERT_SUCCEEDED(g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap)));
        sm_DescriptorHeapPool.emplace_back(heap);
        return heap;
    }



    DescriptorHandle::DescriptorHandle()
    {
        m_cpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
        m_gpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    }

    DescriptorHandle::DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
        : m_cpuHandle(cpuHandle), m_gpuHandle(gpuHandle)
    {
    }

    DescriptorHandle DescriptorHandle::operator+(INT offsetScaledByDescriptorSize) const
    {
        DescriptorHandle ret = *this;
        ret += offsetScaledByDescriptorSize;
        return ret;
    }

    void DescriptorHandle::operator+=(INT offsetScaledByDescriptorSize)
    {
        if (m_cpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN) {
            m_cpuHandle.ptr += offsetScaledByDescriptorSize;
        }
        if (m_gpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN) {
            m_gpuHandle.ptr += offsetScaledByDescriptorSize;
        }
    }




    void DescriptorHeap::Create(const WSTRING& debugHeapName, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT32 maxCount)
    {
        m_heapDesc.Type = type;
        m_heapDesc.NumDescriptors = maxCount;
        m_heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        m_heapDesc.NodeMask = 1;

        ASSERT_SUCCEEDED(g_Device->CreateDescriptorHeap(&m_heapDesc, IID_PPV_ARGS(m_heap.ReleaseAndGetAddressOf())));

#ifdef RELEASE
        void(debugHeapName);
#else
        m_heap->SetName(debugHeapName.c_str());
#endif
        m_descriptorSize = g_Device->GetDescriptorHandleIncrementSize(m_heapDesc.Type);
        m_numFreeDescriptors = m_heapDesc.NumDescriptors;
        m_firstHandle = DescriptorHandle(
            m_heap->GetCPUDescriptorHandleForHeapStart(),
            m_heap->GetGPUDescriptorHandleForHeapStart());
        m_nextFreeHandle = m_firstHandle;
    }

    bool DescriptorHeap::HasAvailableSpace(UINT32 count) const
    {
        return count <= m_numFreeDescriptors;
    }

    DescriptorHandle DescriptorHeap::Alloc(UINT32 count)
    {
        ASSERT(HasAvailableSpace(count), "Descriptor heap out of space! Increase heap size.");
        DescriptorHandle ret = m_nextFreeHandle;
        m_nextFreeHandle += count * m_descriptorSize;
        m_numFreeDescriptors -= count;
        return ret;
    }

    DescriptorHandle DescriptorHeap::operator[](UINT32 arrayIndex) const
    {
        return m_firstHandle + arrayIndex + m_descriptorSize;
    }

    UINT32 DescriptorHeap::GetOffsetOfHandle(const DescriptorHandle& dHandle)
    {
        return (UINT32)(dHandle.GetCpuPtr() - m_firstHandle.GetCpuPtr()) / m_descriptorSize;
    }

    bool DescriptorHeap::ValidateHandle(const DescriptorHandle& dHandle) const
    {
        return false;
    }
}