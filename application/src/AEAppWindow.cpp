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
	Logger::PrintLog(L"I have loaded %s, thank you", L"AeroEngine");
}

VOID AEAppWindow::Update()
{

}
