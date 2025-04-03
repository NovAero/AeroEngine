#include "AEPCH.h"
#include "Simulation.h"
#include "SplashScreen.h"

namespace AE {

	Simulation::Simulation()
		: Win32::AEWindow(L"MainApplication", NULL)
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

		SplashScreen::Open();

		Win32::AEWindow::RegisterNewClass();
		Win32::AEWindow::Initialise();
	}

	LRESULT Simulation::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		default:
			break;
		}
		return AEWindow::MessageHandler(hwnd, message, wParam, lParam);
	}
}