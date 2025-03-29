#include "pch.h"
#include "AEAppWindow.h"
#include "Platform/_WIN32/WinEntry.h"

//Interfacing for entry point... Magic!!
ENTRYAPP(AEAppWindow)

AEAppWindow::AEAppWindow()
{
}

AEAppWindow::~AEAppWindow()
{
}

VOID AEAppWindow::SetupPGS()
{
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);

}

VOID AEAppWindow::Initialise()
{
	Logger::PrintDebugSeperator();
	Logger::PrintLog(L"Application Starting...\n");
	Logger::PrintLog(L"GameName: %s\n", PerGameSettings::GameName());
	Logger::PrintLog(L"Boot Time: %s\n", PerGameSettings::BootTime());
	Logger::PrintDebugSeperator();

	Logger::StartMTail();

}

VOID AEAppWindow::Update()
{
}
