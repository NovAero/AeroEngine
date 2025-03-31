#include "AEPCH.h"
#include "Simulation.h"
#include "SplashScreen.h"

namespace AE {

	Simulation::Simulation()
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


	}
}