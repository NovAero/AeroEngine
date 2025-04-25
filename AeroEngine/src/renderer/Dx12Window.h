#pragma once

#include "Dx12Deps.h"
#include "DXHelper.h"

namespace AE::Graphics {

	using namespace std;

	ComPtr<ID3D12Device2> g_device;

    class AERO_API Dx12Window : public Win32::W32Window {
    public:

        Dx12Window(WSTRING title, HICON icon = nullptr , Win32::EWindowType type = Win32::EWindowType::RESIZEABLE);
        virtual ~Dx12Window();

		virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

        virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnDestroy() = 0;

        virtual void OnKeyDown(UINT8 /*key*/) {}
        virtual void OnKeyUp(UINT8 /*key*/) {}

	public:

		static const UINT						s_FrameCount = 2;

    protected:
        
        WSTRING									GetAssetFullPath(LPCWSTR assetName);

		VOID									InitialisePipeline(D3D12_COMMAND_LIST_TYPE clType, D3D12_DESCRIPTOR_HEAP_TYPE dhType);

		VOID									EnableDebugLayer();
        ComPtr<IDXGIAdapter4>					GetHardwareAdapter(bool useWarp);
		ComPtr<ID3D12Device2>					CreateDevice(ComPtr<IDXGIAdapter4> adapter);
		ComPtr<ID3D12CommandQueue>				CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
		ComPtr<IDXGISwapChain4>					CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue, UINT width, UINT height, UINT bufferCount);
		ComPtr<ID3D12DescriptorHeap>			CreateDescriptorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors);
		VOID									UpdateRenderTargetViews(ComPtr<ID3D12Device2> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap);
		ComPtr<ID3D12CommandAllocator>			CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
		ComPtr<ID3D12GraphicsCommandList>		CreateCommandList(ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type);
		ComPtr<ID3D12Fence>						CreateFence(ComPtr<ID3D12Device> device);
		HANDLE									CreateEventHandle();
		UINT									Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, UINT64 fenceValue);
		VOID									WaitForFenceValue(ComPtr<ID3D12Fence> fence, UINT64 fenceValue, HANDLE fenceEvent, std::chrono::milliseconds duration = std::chrono::milliseconds::max());
		VOID									Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, UINT64& fenceValue, HANDLE fenceEvent);
		VOID									Resize(UINT width, UINT height);
		VOID									SetFullscreen(BOOL fullscreen);

		BOOL									CheckTearingSupport();
		VOID									ParseCmdLineArgs();


        BOOL									m_UseWarpDevice = FALSE;
		WSTRING									m_AssetsPath;
		BOOL									m_Initialised = FALSE;

		// Pipeline objects.
		D3D12_VIEWPORT							m_viewport;
		D3D12_RECT								m_scissorRect;
		ComPtr<ID3D12CommandQueue>				m_commandQueue;
		ComPtr<IDXGISwapChain4>					m_swapChain;
		ComPtr<ID3D12Resource>					m_backBuffers[s_FrameCount];
		ComPtr<ID3D12GraphicsCommandList>		m_commandList;
		ComPtr<ID3D12CommandAllocator>			m_commandAllocators[s_FrameCount];
		ComPtr<ID3D12DescriptorHeap>			m_rtvHeap;
		UINT									m_rtvDescriptorSize;
		UINT									m_currentBufferIndex;


		ComPtr<ID3D12PipelineState> m_pipelineState;
		ComPtr<ID3D12RootSignature> m_rootSignature;

		// App resources.
		ComPtr<ID3D12Resource>					m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW				m_vertexBufferView;

		// Synchronization objects.

		ComPtr<ID3D12Fence>						m_fence;
		UINT64									m_fenceValue;
		UINT64									m_frameFenceValues[s_FrameCount] = {};
		HANDLE									m_fenceEvent;

		BOOL									m_VSync = true;
		BOOL									m_tearingSupported = false;
		BOOL									m_fullscreen = false;

    public:

		ComPtr<ID3D12Device2> Device()	{ return m_device; }
        BOOL WarpDevice()               { return m_UseWarpDevice; }

    public:

        VOID WarpDevice(BOOL use)       { m_UseWarpDevice = use; }
    };
}