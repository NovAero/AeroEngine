#include "AEPCH.h"
#include "Simulation.h"

#include <d3dx12.h>

namespace AE {

	Simulation::Simulation()
		: AE::Renderer::Dx12Window(L"Dx12 Simulation"),
		m_frameIndex(0),
		m_viewport(0.0f, 0.0f, static_cast<float>(Size().cx), static_cast<float>(Size().cy)),
		m_scissorRect(0, 0,Size().cx, Size().cy),
		m_rtvDescriptorSize(0)
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
		OnInit();
	}

	void Simulation::OnInit()
	{
		LoadPipeline();
		LoadAssets();
	}

	void Simulation::OnUpdate()
	{
	}

	void Simulation::OnRender()
	{
		//Record all commands to render the scene
		PopulateCommandList();

		//Execute the commands
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		//Present frame
		ThrowIfFailed(m_swapChain->Present(1, 0));

		WaitForPreviousFrame();
	}

	void Simulation::OnDestroy()
	{
		// Wait for the GPU to be done with all resources.
		WaitForPreviousFrame();

		CloseHandle(m_fenceEvent);
	}

	LRESULT Simulation::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Dx12Window* pSample = reinterpret_cast<Dx12Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		switch (message) {
		case WM_PAINT:
			if (pSample)
			{
				pSample->OnUpdate();
				pSample->OnRender();
			}
		}

		return AEWindow::MessageHandler(hwnd, message, wParam, lParam);
	}


	void Simulation::LoadPipeline()
	{

		UINT dxgiFactoryFlags = 0;

	#ifdef _DEBUG
		//enable debug layer for D3D12

		{
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();

				// Enable additional debug layers.
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
	#endif // _DEBUG

		ComPtr<IDXGIFactory4> factory;
		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

		if (m_UseWarpDevice) {
			ComPtr<IDXGIAdapter> warpAdapter;
			ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

			ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
		}
		else {
			ComPtr<IDXGIAdapter1> hardwareAdapter;
			GetHardwareAdapter(factory.Get(), &hardwareAdapter);

			ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
		}

		//Describe and fill out the command queue
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));


		//Describe and create swap chain
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

		swapChainDesc.Width = Size().cx;
		swapChainDesc.Height = Size().cy;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = FrameCount;
		swapChainDesc.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

		ComPtr<IDXGISwapChain1> swapChain;
		ThrowIfFailed(factory->CreateSwapChainForHwnd(m_commandQueue.Get(), Handle(), &swapChainDesc, nullptr, nullptr, &swapChain));

		ThrowIfFailed(swapChain.As(&m_swapChain));

		// This sample does not support fullscreen transitions. 
		ThrowIfFailed(factory->MakeWindowAssociation(Handle(), DXGI_MWA_NO_ALT_ENTER));

		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

		//create descriptor heaps
		{
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = FrameCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
		}

		//create frame resources
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

			for (UINT n = 0; n < FrameCount; ++n) {
				ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
				m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
				rtvHandle.Offset(1, m_rtvDescriptorSize);
			}
		}

		ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
	}

	void Simulation::LoadAssets()
	{
		//Create empty root signiature for PSO
		{
			CD3DX12_ROOT_SIGNATURE_DESC rootSigniatureDesc;
			rootSigniatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ComPtr<ID3DBlob> signiature;
			ComPtr<ID3DBlob> error;

			ThrowIfFailed(D3D12SerializeRootSignature(&rootSigniatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, 
														&signiature, &error));
			ThrowIfFailed(m_device->CreateRootSignature(0, signiature->GetBufferPointer(), 
														signiature->GetBufferSize(), IID_PPV_ARGS(&m_rootSigniature)));

		}
		//Create pipeline state, includes compiling and loading shaders
		{
			ComPtr<ID3DBlob> vertexShader;
			ComPtr<ID3DBlob> pixelShader;

		#ifdef _DEBUG
			UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		#else
			UINT compileFlags = 0;
		#endif // _DEBUG
			
			//Attempt loading of vertex shader
			ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"shaders.hlsl").c_str(), nullptr, nullptr,
											 "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
			//Attempt loading of pixel shader
			ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"shaders.hlsl").c_str(), nullptr, nullptr,
				"PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

			//Define vertexShader input layout
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};

			// Describe and create the graphics pipeline state object (PSO).
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_rootSigniature.Get();
			psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
			psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = FALSE;
			psoDesc.DepthStencilState.StencilEnable = FALSE;
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			psoDesc.SampleDesc.Count = 1;

			ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
		}

		ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));
	
		// Command lists are created in the recording state, but there is nothing
		// to record yet. The main loop expects it to be closed, so close it now.
		ThrowIfFailed(m_commandList->Close());
		
		//Create vertex buffer
		{
			//Define triangle geometry
			Vertex triangleVertices[] =
			{
				{ { 0.0f, 0.25f * AsRatio(), 0.0f	 },	{ 1.0f, 0.0f, 0.0f, 1.0f } },
				{ { 0.25f, -0.25f * AsRatio(), 0.0f  },	{ 0.0f, 1.0f, 0.0f, 1.0f } },
				{ { -0.25f, -0.25f * AsRatio(), 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
			};

			const UINT vertexBufferSize = sizeof(triangleVertices);

			//NOTE: upload heaps are bad, replace with Default heap later, its ok here since there are only a few verts
			CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
			auto desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
			ThrowIfFailed(m_device->CreateCommittedResource(
				&heapProps,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_vertexBuffer)));

			// Copy the triangle data to the vertex buffer.
			UINT8* pVertexDataBegin;
			CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
			ThrowIfFailed(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
			memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
			m_vertexBuffer->Unmap(0, nullptr);

			// Initialize the vertex buffer view.
			m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
			m_vertexBufferView.StrideInBytes = sizeof(Vertex);
			m_vertexBufferView.SizeInBytes = vertexBufferSize;
		}

		//Create sync object and wait until assets have loaded to GPU
		{
			ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
			m_fenceValue = 1;

			//create an event handle to use for fram sync
			m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (m_fenceEvent == nullptr) {
				ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
			}

			//Wait for the command list to finish execute
			//Reusing the same command list in main loop, for now just wait for setup to complete
			//before continuing
			WaitForPreviousFrame();
		}
	}

	void Simulation::PopulateCommandList()
	{
		// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
		ThrowIfFailed(m_commandAllocator->Reset());

		// However, when ExecuteCommandList() is called on a particular command 
		// list, that command list can then be reset at any time and must be before 
		// re-recording.
		ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));

		// Set necessary state.
		m_commandList->SetGraphicsRootSignature(m_rootSigniature.Get());
		m_commandList->RSSetViewports(1, &m_viewport);
		m_commandList->RSSetScissorRects(1, &m_scissorRect);

		// Indicate that the back buffer will be used as a render target.
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_commandList->ResourceBarrier(1, &barrier);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
		m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		// Record commands.
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
		m_commandList->DrawInstanced(3, 1, 0, 0);

		// Indicate that the back buffer will now be used to present.
		barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_commandList->ResourceBarrier(1, &barrier);

		ThrowIfFailed(m_commandList->Close());
	}

	void Simulation::WaitForPreviousFrame()
	{
		// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. More advanced samples 
	// illustrate how to use fences for efficient resource usage.

	// Signal and increment the fence value.
		const UINT64 fence = m_fenceValue;
		ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
		m_fenceValue++;

		// Wait until the previous frame is finished.
		if (m_fence->GetCompletedValue() < fence)
		{
			ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}

		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
	}
}