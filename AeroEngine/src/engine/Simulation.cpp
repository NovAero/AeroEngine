#include "AEPCH.h"
#include "Simulation.h"

namespace AE {

	Simulation::Simulation()
		: AE::Renderer::Dx12Window(L"Dx12 Simulation")
	{

	}

	Simulation::~Simulation()
	{
	}


	VOID Simulation::PreInitialise()
	{
		Logger::PrintLog(L"Application Starting...\n");
		Logger::PrintLog(L"GameName: %s\n", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s\n", PerGameSettings::BootTime());
		Logger::PrintLog(L"Engine Mode: %s\n", Engine::EngineModeToString().c_str());

		Win32::AEWindow::RegisterNewClass();
		Win32::AEWindow::Initialise();
		OnInit();
	}

	VOID Simulation::Exit()
	{
		OnDestroy();
	}

	void Simulation::OnInit()
	{
		EnableDebugLayer();
		m_tearingSupported = CheckTearingSupport();

		ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetHardwareAdapter(m_UseWarpDevice);

		m_device = CreateDevice(dxgiAdapter4);

		m_commandQueue = CreateCommandQueue(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);

		m_swapChain = CreateSwapChain(Handle(), m_commandQueue, Size().cx, Size().cy, FrameCount);

		m_currentBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

		m_rtvHeap = CreateDescriptorHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FrameCount);
		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		UpdateRenderTargetViews(m_device, m_swapChain, m_rtvHeap);

		for (int i = 0; i < FrameCount; ++i) {
			m_commandAllocators[i] = CreateCommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
		}

		m_commandList = CreateCommandList(m_device, m_commandAllocators[m_currentBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);

		m_fence = CreateFence(m_device);
		m_fenceEvent = CreateEventHandle();

		m_Initialised = TRUE;

		::ShowWindow(Handle(), SW_SHOW);
	}

	void Simulation::OnUpdate()
	{
		static uint64_t frameCounter = 0;
		static double elapsedSeconds = 0.0;
		static std::chrono::high_resolution_clock clock;
		static auto t0 = clock.now(); 
		
		frameCounter++;
		auto t1 = clock.now();
		auto deltaTime = t1 - t0;
		t0 = t1;
		elapsedSeconds += deltaTime.count() * 1e-9;

		if (elapsedSeconds > 1.0)
		{
			WCHAR buffer[500];
			auto fps = frameCounter / elapsedSeconds;
			fps *= 1e-05;
			swprintf_s(buffer, L"FPS: %f\n", fps);
			Logger::PrintLog(buffer);

			frameCounter = 0;
			elapsedSeconds = 0.0;
		}
	}

	void Simulation::OnRender()
	{
		auto commandAllocator = m_commandAllocators[m_currentBufferIndex];
		auto backBuffer = m_backBuffers[m_currentBufferIndex];

		commandAllocator->Reset();
		m_commandList->Reset(commandAllocator.Get(), nullptr);

		//Clear the render target
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			m_commandList->ResourceBarrier(1, &barrier);

			FLOAT clearColour[] = { 0.4f, 0.6f, 0.9f, 1.0f };
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currentBufferIndex, m_rtvDescriptorSize);

			m_commandList->ClearRenderTargetView(rtv, clearColour, 0, nullptr);

		}

		// Present
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(),D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
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

	void Simulation::OnDestroy()
	{
		Flush(m_commandQueue, m_fence, m_fenceValue, m_fenceEvent);

		::CloseHandle(m_fenceEvent);
	}

	LRESULT Simulation::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(m_Initialised)
		{
			
		}

		return Dx12Window::MessageHandler(hwnd, message, wParam, lParam);
	}


}