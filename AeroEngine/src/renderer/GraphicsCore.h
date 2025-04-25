#pragma once

//uncomment when added
//#include "PipelineState.h"
#include "DescriptorHeap.h"
#include "RootSigniature.h"
#include "GraphicsCommon.h"

class CommandListManager;
class ContextManager;

namespace AE::Graphics {

#ifndef RELEASE
	extern const GUID WKPDID_D3DebugObjectName;
#endif

	using namespace Microsoft::WRL;

	/*void Initialise(bool requireDXRSupport = false);
	void Shutdown(void);

	bool IsDeviceNvidia(ID3D12Device* device);
	bool IsDeviceAMD(ID3D12Device* device);
	bool IsDeviceIntel(ID3D12Device* device);
	*/


	extern ComPtr<ID3D12Device2> g_Device;
	extern CommandListManager g_CommandManager;
	extern ContextManager g_ContextManager;

	extern D3D_FEATURE_LEVEL g_D3DFeatureLevel;
	extern bool g_TypedUAVLoadSupport_R11G11B10_FLOAT;
	extern bool g_TypedUAVLoadSupport_R16G16B16A16_FLOAT;

	extern DescriptorAllocator g_DescriptorAllocator[];
	inline D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT count = 1) {
		return g_DescriptorAllocator[type].Allocate(count);
	}
}