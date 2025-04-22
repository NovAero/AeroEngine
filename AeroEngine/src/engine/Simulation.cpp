#include "AEPCH.h"
#include "Simulation.h"

namespace AE {

	Graphics::Renderer* Simulation::s_RenderManager;

	Simulation::Simulation()
	{
		s_RenderManager = new Graphics::Renderer(L"test", 600, 600, TRUE);
	}

	Simulation::~Simulation()
	{
		delete s_RenderManager;
	}


	VOID Simulation::Register()
	{
		Logger::PrintLog(L"Application Starting...\n");
		Logger::PrintLog(L"GameName: %s\n", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s\n", PerGameSettings::BootTime());
		Logger::PrintLog(L"Engine Mode: %s\n", Engine::EngineModeToString().c_str());

		s_RenderManager->Register();
	}

	VOID Simulation::Exit()
	{
		s_RenderManager->Exit();
	}

	void Simulation::OnInit()
	{
		s_RenderManager->OnInit();
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
		s_RenderManager->OnRender();
	}

	void Simulation::OnDestroy()
	{
		
	}

	VOID Simulation::OnKeyPressed()
	{
	}

	VOID Simulation::OnKeyReleased()
	{
	}

	VOID Simulation::OnMouseMove()
	{
	}

	VOID Simulation::OnMouseButtonPressed()
	{
	}

	VOID Simulation::OnMouseWheel()
	{
	}

	VOID Simulation::OnResize()
	{
	}

	VOID Simulation::OnWindowDestroy()
	{
	}



}