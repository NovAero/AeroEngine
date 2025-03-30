#include "pch.h"
#include "AEApplication.h"
#include "Platform/_WIN32/WinEntry.h"

//Interfacing for entry point... Magic!!
ENTRYAPP(AEApplication)

AEApplication::AEApplication()
{
}

AEApplication::~AEApplication()
{
}

VOID AEApplication::SetupPGS()
{
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);

}

VOID AEApplication::Initialise()
{
	Logger::PrintLog(L"Application Starting...\n");
	Logger::PrintLog(L"GameName: %s\n", PerGameSettings::GameName());
	Logger::PrintLog(L"Boot Time: %s\n", PerGameSettings::BootTime());
	Logger::PrintLog(L"Engine Mode: %s\n", Engine::EngineModeToString().c_str());

}

VOID AEApplication::Update()
{
}
