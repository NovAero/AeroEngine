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

VOID AEAppWindow::Initialise()
{
	MessageBox(0, L"I have loaded", 0, 0);
}

VOID AEAppWindow::Update()
{
	MessageBox(0, L"Loop", 0, 0);
}
