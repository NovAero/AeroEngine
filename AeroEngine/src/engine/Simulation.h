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

		virtual bool LoadContent();
		virtual bool UnloadContent();

	protected:

		virtual VOID OnKeyPressed(/*Keyeventargs& e*/);
		virtual VOID OnKeyReleased(/*Keyeventargs& e*/);
		virtual VOID OnMouseMove(/*Mouseeventargs& e*/);
		virtual VOID OnMouseButtonPressed(/*Mouseeventargs& e*/);
		virtual VOID OnMouseWheel(/*Mouseeventargs& e*/);

		virtual VOID OnResize();
		virtual VOID OnWindowDestroy();

		static Graphics::Renderer* s_RenderManager;

	};

}