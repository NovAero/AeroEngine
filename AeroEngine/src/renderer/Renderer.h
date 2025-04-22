#pragma once

#include "renderer/Dx12Window.h"

namespace AE::Graphics {

	class AERO_API Renderer : public AE::Graphics::Dx12Window{
	public:
		Renderer(const WSTRING name, INT width, INT height, BOOL vSync);
		~Renderer();

		virtual VOID Register();
		virtual VOID Exit();

		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();

		virtual void LoadContent();
		virtual void UnloadContent();

		virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:

	};
}