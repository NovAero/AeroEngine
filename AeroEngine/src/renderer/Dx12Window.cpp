#include "AEPCH.h"
#include "Dx12Window.h"

namespace AE::Renderer {

    Dx12Window::Dx12Window(WSTRING title, HICON icon, Win32::AEWindowType type)
        : Win32::AEWindow(title, icon, type)
    {
        WCHAR assetsPath[512];
        GetAssetsPath(assetsPath, _countof(assetsPath));
        m_AssetsPath = assetsPath;
    }

    AE::Renderer::Dx12Window::~Dx12Window()
    {
    }

    WSTRING Dx12Window::GetAssetFullPath(LPCWSTR assetName)
    {
        return m_AssetsPath + assetName;
    }

    // Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
    // If no such adapter can be found, *ppAdapter will be set to nullptr.
    _Use_decl_annotations_
    void Dx12Window::GetHardwareAdapter(
            IDXGIFactory1* pFactory,
            IDXGIAdapter1** ppAdapter,
            bool requestHighPerformanceAdapter)
    {

        using namespace Microsoft::WRL;

        *ppAdapter = nullptr;

        ComPtr<IDXGIAdapter1> adapter;

        ComPtr<IDXGIFactory6> factory6;
        if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
        {
            for (
                UINT adapterIndex = 0;
                SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                    adapterIndex,
                    requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                    IID_PPV_ARGS(&adapter)));
                    ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    // Don't select the Basic Render Driver adapter.
                    // If you want a software adapter, pass in "/warp" on the command line.
                    continue;
                }

                // Check to see whether the adapter supports Direct3D 12, but don't create the
                // actual device yet.
                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        if (adapter.Get() == nullptr)
        {
            for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    // Don't select the Basic Render Driver adapter.
                    // If you want a software adapter, pass in "/warp" on the command line.
                    continue;
                }

                // Check to see whether the adapter supports Direct3D 12, but don't create the
                // actual device yet.
                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        *ppAdapter = adapter.Detach();
    }

}