#pragma once

#include "renderer/Dx12Window.h"
#include "Dx12Deps.h"

using namespace DirectX;

namespace AE::Graphics {

	class AERO_API Renderer : public AE::Graphics::Dx12Window{
	public:
		Renderer(const WSTRING name, INT width, INT height, BOOL vSync);

		virtual VOID Register();
		virtual VOID Exit();

		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();

		virtual void LoadContent();
		void WaitForPreviousFrame();
		virtual void UnloadContent();

		void PopulateCommandList();

		virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		struct Vertex
		{
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

	};
}