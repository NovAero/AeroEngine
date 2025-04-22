#pragma once

#include "renderer/Renderer.h"

//Aero engine Simulation class is the main loadbearing class,
//containing renderer, game logic, handlers, input systems, etc.

namespace AE {

	class AERO_API Simulation : public std::enable_shared_from_this<Simulation>, public Win32::IAEApplication {
	public:
		Simulation();
		virtual ~Simulation();
		
		virtual VOID Register() override;
		virtual VOID Exit() override;

		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();

		virtual void LoadContent();
		virtual void UnloadContent();

	protected:

		virtual VOID OnKeyPressed(/*Keyeventargs& */);
		virtual VOID OnKeyReleased();
		virtual VOID OnMouseMove();
		virtual VOID OnMouseButtonPressed();
		virtual VOID OnMouseWheel();

		virtual VOID OnResize();
		virtual VOID OnWindowDestroy();

	private:

		static Graphics::Renderer* s_RenderManager;

	};

}