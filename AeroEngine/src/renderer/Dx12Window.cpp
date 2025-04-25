#include "AEPCH.h"
#include "Dx12Window.h"
#include <dxgidebug.h>

namespace AE::Graphics {

    Dx12Window::Dx12Window(WSTRING title, HICON icon, Win32::EWindowType type)
        : Win32::W32Window(title, icon, type)
        
    {
        WCHAR assetsPath[512];
        GetAssetsPath(assetsPath, _countof(assetsPath));
        m_AssetsPath = assetsPath;
    }

    AE::Graphics::Dx12Window::~Dx12Window()
    {
    }

    LRESULT Dx12Window::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (m_Initialised) {
            switch (message) {
            case WM_PAINT:          { OnUpdate(); OnRender(); }  break;
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:
            {
                bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

                switch (wParam)
                {
                case 'V':
                    m_VSync = !m_VSync;
                    break;
                case VK_ESCAPE:
                    ::PostQuitMessage(0);
                    break;
                case VK_RETURN:
                    if (alt)
                    {
                case VK_F11:
                    SetFullscreen(!m_fullscreen);
                    }
                    break;
                }
            }
            break;
            case WM_SYSCHAR: break;
            case WM_SIZE:
            {
                RECT clientRect = {};
                ::GetClientRect(Handle(), &clientRect);

                int width = clientRect.right - clientRect.left;
                int height = clientRect.bottom - clientRect.top;

                Resize(width, height);
            } break;
            }
        }
            
        return W32Window::MessageHandler(hwnd, message, wParam, lParam);
    }

    WSTRING Dx12Window::GetAssetFullPath(LPCWSTR assetName)
    {
        return m_AssetsPath + assetName;
    }

    VOID Dx12Window::InitialisePipeline(D3D12_COMMAND_LIST_TYPE clType, D3D12_DESCRIPTOR_HEAP_TYPE dhType)
    {
        ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetHardwareAdapter(m_UseWarpDevice);
        m_device = CreateDevice(dxgiAdapter4);

        m_commandQueue = CreateCommandQueue(m_device, clType);

        m_swapChain = CreateSwapChain(Handle(), m_commandQueue, Size().cx, Size().cy, s_FrameCount);
        m_currentBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

        m_rtvHeap = CreateDescriptorHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, s_FrameCount);
        m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        UpdateRenderTargetViews(m_device, m_swapChain, m_rtvHeap);

        for (int i = 0; i < s_FrameCount; ++i) {
            m_commandAllocators[i] = CreateCommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
        }

        m_commandList = CreateCommandList(m_device, m_commandAllocators[m_currentBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);

        m_fence = CreateFence(m_device);
        m_fenceEvent = CreateEventHandle();

        m_Initialised = TRUE;
    }

    VOID Dx12Window::EnableDebugLayer()
    {

    #ifdef _DEBUG

        ComPtr<ID3D12Debug> debugInterface;
        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));

        debugInterface->EnableDebugLayer();

    #endif // _DEBUG
    }

    ComPtr<IDXGIAdapter4> Dx12Window::GetHardwareAdapter(bool useWarp)
    {
        ComPtr<IDXGIFactory4> dxgiFactory;

        UINT createFactoryFlags = 0;

    #ifdef _DEBUG
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
    #endif // _DEBUG

        ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

        ComPtr<IDXGIAdapter1> dxgiAdapter1;
        ComPtr<IDXGIAdapter4> dxgiAdapter4;

        if (useWarp) {
            ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
            ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));

        }
        else {

            SIZE_T maxDedicatedVideoMemory = 0;

            for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i) {

                DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
                dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

                if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE) == 0
                    && SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))
                    && dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
                {
                    maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
                    ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));

                }

            }
        }

        return dxgiAdapter4;
    }

    VOID Dx12Window::ParseCmdLineArgs() {
        int argc;

        wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

        for (size_t i = 0; i < argc; ++i)
        {
            if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0)
            {
                WarpDevice(true);
            }
        }
        // Free memory allocated by CommandLineToArgvW
        ::LocalFree(argv);
    }

    ComPtr<ID3D12Device2> Dx12Window::CreateDevice(ComPtr<IDXGIAdapter4> adapter)
    {
        ComPtr<ID3D12Device2> d3d12Device2;

        ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)));

    #ifdef _DEBUG
        ComPtr<ID3D12InfoQueue> pInfoQueue;
        if (SUCCEEDED(d3d12Device2.As(&pInfoQueue))) {
            pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
            pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
            pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

            //Suppress messages based on severity
            D3D12_MESSAGE_SEVERITY Severities[] =
            {
                D3D12_MESSAGE_SEVERITY_INFO
            };

            //Supress invdiviual messages by their ID
            D3D12_MESSAGE_ID DenyIds[] = {
                D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
                D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
                D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
            };

            D3D12_INFO_QUEUE_FILTER NewFilter = {};
            NewFilter.DenyList.NumSeverities = _countof(Severities);
            NewFilter.DenyList.pSeverityList = Severities;
            NewFilter.DenyList.NumIDs = _countof(DenyIds);
            NewFilter.DenyList.pIDList = DenyIds;

            ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));
        }
    #endif // _DEBUG

        return d3d12Device2;
    }

    ComPtr<ID3D12CommandQueue> Dx12Window::CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
    {
        ComPtr<ID3D12CommandQueue> d3d12CommandQueue;

        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = type;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;

        ThrowIfFailed(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue)));

        return d3d12CommandQueue;
    }

    ComPtr<IDXGISwapChain4> Dx12Window::CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue, UINT width, UINT height, UINT bufferCount)
    {
        ComPtr<IDXGISwapChain4> dxgiSwapChain4;
        ComPtr<IDXGIFactory4> dxgiFactory4;
        UINT createFactoryFlags;
    #ifdef _DEBUG
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
    #endif // 

        ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.Stereo = FALSE;
        swapChainDesc.SampleDesc = { 1,0 };
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = bufferCount;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

        ComPtr<IDXGISwapChain1> swapChain1;
        ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(commandQueue.Get(),
            hWnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain1));

        ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

        ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));

        return dxgiSwapChain4;

    }

    ComPtr<ID3D12DescriptorHeap> AE::Graphics::Dx12Window::CreateDescriptorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors)
    {
        ComPtr<ID3D12DescriptorHeap> descriptorHeap;

        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = numDescriptors;
        desc.Type = type;

        ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

        return descriptorHeap;
    }

    VOID Dx12Window::UpdateRenderTargetViews(ComPtr<ID3D12Device2> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap)
    {

        auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

        for (int i = 0; i < s_FrameCount; ++i) {
            ComPtr<ID3D12Resource> backBuffer;
            ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

            device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

            m_backBuffers[i] = backBuffer;

            rtvHandle.Offset(rtvDescriptorSize);
        }

    }

    ComPtr<ID3D12CommandAllocator> Dx12Window::CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
    {
        ComPtr<ID3D12CommandAllocator> commandAllocator;
        ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));

        return commandAllocator;
    }

    ComPtr<ID3D12GraphicsCommandList> Dx12Window::CreateCommandList(ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type)
    {
        ComPtr<ID3D12GraphicsCommandList> commandList;
        ThrowIfFailed(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

        ThrowIfFailed(commandList->Close());

        return commandList;
    }

    ComPtr<ID3D12Fence> Dx12Window::CreateFence(ComPtr<ID3D12Device> device)
    {
        ComPtr<ID3D12Fence> fence;
        ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

        return fence;
    }

    HANDLE Dx12Window::CreateEventHandle()
    {
        HANDLE fenceEvent;

        fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        assert(fenceEvent && "Failed to create fence event");

        return fenceEvent;
    }

    UINT Dx12Window::Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, UINT64 fenceValue)
    {
        UINT fenceValueForSignal = ++fenceValue;
        ThrowIfFailed(commandQueue->Signal(fence.Get(), fenceValueForSignal));

        return fenceValueForSignal;
    }

    VOID Dx12Window::WaitForFenceValue(ComPtr<ID3D12Fence> fence, UINT64 fenceValue, HANDLE fenceEvent, std::chrono::milliseconds duration)
    {
        if (fence->GetCompletedValue() < fenceValue) {
            ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
            ::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
        }
    }

    VOID Dx12Window::Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, UINT64& fenceValue, HANDLE fenceEvent)
    {
        UINT64 fenceValueForSignal = Signal(commandQueue, fence, fenceValue);
        WaitForFenceValue(fence, fenceValueForSignal, fenceEvent);
    }

    VOID Dx12Window::Resize(UINT width, UINT height)
    {
        if (Size().cx != width || Size().cy != height) {
            //Size cant be zero
            Size(std::max(1u, width), std::max(1u, height));

            Flush(m_commandQueue, m_fence, m_fenceValue, m_fenceEvent);

            for (int i = 0; i < s_FrameCount; ++i) {

                m_backBuffers[i].Reset();
                m_frameFenceValues[i] = m_frameFenceValues[m_currentBufferIndex];
            }

            DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
            ThrowIfFailed(m_swapChain->GetDesc(&swapChainDesc));
            ThrowIfFailed(m_swapChain->ResizeBuffers(s_FrameCount, Size().cx, Size().cy, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));

            m_currentBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

            UpdateRenderTargetViews(m_device, m_swapChain, m_rtvHeap);
        }
    }

    VOID Dx12Window::SetFullscreen(BOOL fullscreen)
    {
        if (m_fullscreen != fullscreen) {
            m_fullscreen = fullscreen;

            if (m_fullscreen) {

                RECT rect;

                GetWindowRect(Handle(), &rect);

                UINT windowStyle = Win32::EWindowType::POPUP;

                SetWindowLongW(Handle(), GWL_STYLE, windowStyle);

                // Query the name of the nearest display device for the window.
                // This is required to set the fullscreen dimensions of the window
                // when using a multi-monitor setup.
                HMONITOR hMonitor = ::MonitorFromWindow(Handle(), MONITOR_DEFAULTTONEAREST);
                MONITORINFOEX monitorInfo = {};
                monitorInfo.cbSize = sizeof(MONITORINFOEX);
                GetMonitorInfo(hMonitor, &monitorInfo);

                SetWindowPos(Handle(), HWND_TOP,
                    monitorInfo.rcMonitor.left,
                    monitorInfo.rcMonitor.top,
                    monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                    monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
                    SWP_FRAMECHANGED | SWP_NOACTIVATE);

                ShowWindow(Handle(), SW_MAXIMIZE);
            }
            else {
                // Restore all the window decorators.
                SetWindowLong(Handle(), GWL_STYLE, Win32::EWindowType::STATIC_EX);

                SetWindowPos(Handle(), HWND_NOTOPMOST,
                    m_windowRect.left,
                    m_windowRect.top,
                    m_windowRect.right - m_windowRect.left,
                    m_windowRect.bottom - m_windowRect.top,
                    SWP_FRAMECHANGED | SWP_NOACTIVATE);

                ShowWindow(Handle(), SW_NORMAL);

            }
        }
    }

    BOOL Dx12Window::CheckTearingSupport()
    {
        BOOL allowTearing = FALSE;

        ComPtr<IDXGIFactory4> factory4;
        if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
        {
            ComPtr<IDXGIFactory5> factory5;
            if (SUCCEEDED(factory4.As(&factory5)))
            {
                if (FAILED(factory5->CheckFeatureSupport(
                    DXGI_FEATURE_PRESENT_ALLOW_TEARING,
                    &allowTearing, sizeof(allowTearing))))
                {
                    allowTearing = TRUE;
                }
            }
        }

        return allowTearing == TRUE;
    }

}