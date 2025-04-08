#pragma once

#include "renderer/Dx12Window.h"

using namespace Microsoft::WRL;
using namespace DirectX;

namespace AE {

	class AERO_API Simulation : public Win32::IAEApplication, public AE::Renderer::Dx12Window {
	public:
		Simulation();
		~Simulation();
		
		virtual VOID PreInitialise() override;

		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();

		virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:

		static const UINT						FrameCount = 2;

		struct Vertex
		{
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

		// Pipeline objects.
		D3D12_VIEWPORT							m_viewport;
		D3D12_RECT								m_scissorRect;
		ComPtr<IDXGISwapChain3>					m_swapChain;
		ComPtr<ID3D12Device>					m_device;
		ComPtr<ID3D12Resource>					m_renderTargets[FrameCount];
		ComPtr<ID3D12CommandAllocator>			m_commandAllocator;
		ComPtr<ID3D12CommandQueue>				m_commandQueue;
		ComPtr<ID3D12RootSignature>				m_rootSigniature;
		ComPtr<ID3D12DescriptorHeap>			m_rtvHeap;
		ComPtr<ID3D12PipelineState>				m_pipelineState;
		ComPtr<ID3D12GraphicsCommandList>		m_commandList;
		UINT									m_rtvDescriptorSize;

		// App resources.
		ComPtr<ID3D12Resource>					m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW				m_vertexBufferView;

		// Synchronization objects.
		UINT									m_frameIndex;
		HANDLE									m_fenceEvent;
		ComPtr<ID3D12Fence>						m_fence;
		UINT64									m_fenceValue;

		void LoadPipeline();
		void LoadAssets();
		void PopulateCommandList();
		void WaitForPreviousFrame();
	};
}