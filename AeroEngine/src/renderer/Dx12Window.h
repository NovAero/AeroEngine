#pragma once

#include "Dx12Deps.h"
#include "DXHelper.h"

namespace AE::Renderer {
    class Dx12Window : public Win32::AEWindow {
    public:

        Dx12Window(WSTRING title, HICON icon = nullptr , Win32::AEWindowType type = Win32::AEWindowType::RESIZEABLE);
        virtual ~Dx12Window();

        virtual void OnInit() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnDestroy() = 0;

        virtual void OnKeyDown(UINT8 /*key*/) {}
        virtual void OnKeyUp(UINT8 /*key*/) {}

    protected:
        
        WSTRING GetAssetFullPath(LPCWSTR assetName);

        VOID GetHardwareAdapter(
            _In_ IDXGIFactory1* pFactory,
            _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
            bool requestHighPerformanceAdapter = false);

        BOOL        m_UseWarpDevice = FALSE;
        WSTRING     m_AssetsPath;
 
    public:

        BOOL WarpDevice()               { return m_UseWarpDevice; }

    public:

        VOID WarpDevice(BOOL use)       { m_UseWarpDevice = use; }
    };
}