#include "AEPCH.h"
#include "Renderer.h"

using namespace Microsoft::WRL;
using namespace DirectX;

namespace AE::Graphics {
	Renderer::Renderer(const WSTRING name, INT width, INT height, BOOL vSync)
		: Dx12Window(name)
	{
		Size(width, height);
		m_VSync = vSync;
	}

	VOID Renderer::Register()
	{
		Win32::W32Window::RegisterNewClass();
		Win32::W32Window::Initialise();
	}

	VOID Renderer::Exit()
	{
		OnDestroy();
	}

	void Renderer::OnInit()
	{
		EnableDebugLayer();
		m_tearingSupported = CheckTearingSupport();

		ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetHardwareAdapter(m_UseWarpDevice);

		m_device = CreateDevice(dxgiAdapter4);

		m_commandQueue = CreateCommandQueue(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);

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

		::ShowWindow(Handle(), SW_SHOW);
	}

	void Renderer::OnUpdate()
	{

	}

	void Renderer::OnRender()
	{
		auto commandAllocator = m_commandAllocators[m_currentBufferIndex];
		auto backBuffer = m_backBuffers[m_currentBufferIndex];

		commandAllocator->Reset();
		m_commandList->Reset(commandAllocator.Get(), nullptr);

		//Clear the render target
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			m_commandList->ResourceBarrier(1, &barrier);

			FLOAT clearColour[] = { 0.5, 0, 0};
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currentBufferIndex, m_rtvDescriptorSize);

			m_commandList->ClearRenderTargetView(rtv, clearColour, 0, nullptr);

		}

		// Present
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			m_commandList->ResourceBarrier(1, &barrier);

			ThrowIfFailed(m_commandList->Close());

			ID3D12CommandList* const commandLists[] = {
				m_commandList.Get()
			};
			m_commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

			UINT syncInterval = m_VSync ? 1 : 0;
			UINT presentFlags = m_tearingSupported && !m_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
			ThrowIfFailed(m_swapChain->Present(syncInterval, presentFlags));

			m_frameFenceValues[m_currentBufferIndex] = Signal(m_commandQueue, m_fence, m_fenceValue);

			m_currentBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

			WaitForFenceValue(m_fence, m_frameFenceValues[m_currentBufferIndex], m_fenceEvent);
		}
	}

	void Renderer::OnDestroy()
	{
		Flush(m_commandQueue, m_fence, m_fenceValue, m_fenceEvent);
		m_fence->Release();

		m_device->Release();

		m_commandQueue->Release();

		m_swapChain->SetFullscreenState(FALSE, nullptr);
		m_swapChain->Release();

		for (auto buff : m_backBuffers) {
			buff->Release();
		}

		m_commandList->Release();

		for (auto aloc : m_commandAllocators) {
			aloc->Release();
		}

		m_rtvHeap->Release();

		::CloseHandle(m_fenceEvent);
	}

	void Renderer::LoadContent()
	{
	}

	void Renderer::UnloadContent()
	{
	}

	LRESULT Renderer::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (m_Initialised)
		{

		}

		return Dx12Window::MessageHandler(hwnd, message, wParam, lParam);
	}
}