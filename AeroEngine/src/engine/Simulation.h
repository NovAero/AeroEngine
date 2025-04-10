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
		virtual VOID Exit() override;

		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();

		virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:

		struct Vertex
		{
			XMFLOAT3 position;
			XMFLOAT4 color;
		};


	};
}