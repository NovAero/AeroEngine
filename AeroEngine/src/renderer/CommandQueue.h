#pragma once 

#include <d3d12.h>
#include <wrl.h>

#include <cstdint>
#include <queue>

using namespace Microsoft::WRL;

namespace AE::Renderer {
	class CommandQueue {
	public:
		CommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
		virtual ~CommandQueue();

		ComPtr<ID3D12GraphicsCommandList2>	GetCommandList();

		UINT64								ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList);
		
		UINT64								Signal();
		BOOL								IsFenceComplete(UINT64 fenceValue);
		VOID								WaitForFenceValue(UINT64 fenceValue);
		VOID								Flush();

		ComPtr<ID3D12CommandQueue>			GetD3D12CommandQueue() const;

	protected:
		ComPtr<ID3D12CommandAllocator> CreatecommandAllocator();
		ComPtr<ID3D12GraphicsCommandList2> CreateCommandList(ComPtr<ID3D12CommandAllocator> allocator);

	private:
		//Keeping track of command allocators that are "in flight"
		struct CommandAllocatorEntry {
			UINT64 fenceValue;
			ComPtr<ID3D12CommandAllocator> commandAllocator;
		};

		using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
		using CommandListQueue = std::queue<ComPtr<ID3D12GraphicsCommandList2>>;

		D3D12_COMMAND_LIST_TYPE					m_CommandListType;
		ComPtr<ID3D12Device2>					m_d3d12Device;
		ComPtr<ID3D12CommandQueue>				m_d3d12CommandQueue;
		ComPtr<ID3D12Fence>						m_d3d12Fence;
		HANDLE									m_FenceEvent;
		UINT64									m_FenceValue;

		CommandAllocatorQueue					m_CommandAllocatorQueue;
		CommandListQueue						m_CommandListQueue;
	};
}