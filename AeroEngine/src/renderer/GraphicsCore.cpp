#include "AEPCH.h"
#include "GraphicsCore.h"

namespace AE::Graphics {

#ifndef RELEASE
	const GUID WKPDID_D3DDebugObjectName = { 0x429b8c22,0x9188,0x4b0c, { 0x87,0x42,0xac,0xb0,0xbf,0x85,0xc2,0x00 } };
#endif

	bool g_bTypedUAVLoadSupport_R11G11B10_FLOAT = false;
	bool g_bTypedUAVLoadSupport_R16G16B16A16_FLOAT = false;

	ComPtr<ID3D12Device2> g_Device = nullptr;
	CommandListManager g_CommandManager;
	ContextManager g_ContextManager;

	D3D_FEATURE_LEVEL g_D3DFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	DescriptorAllocator g_DescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] =
	{
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV
	};

	static const UINT32 vendorID_Nvidia = 0x10DE;
	static const UINT32 vendorID_AMD = 0x1002;
	static const UINT32 vendorID_Intel = 0x8086;
}