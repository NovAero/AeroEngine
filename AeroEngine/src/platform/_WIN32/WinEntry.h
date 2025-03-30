#include "AEPCH.h"
#include "IAEApplication.h"
#include "common/CmdLineArgs.h"

extern Win32::IAEApplication* EntryApplication();

INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {

	auto EntryApp = EntryApplication();

	PerGameSettings settings;
	EntryApp->SetupPGS();

	CmdLineArgs::ReadArgs();

	Logger logger;
	EntryApp->Initialise();

	MSG msg = { 0 };

	while (msg.message != WM_QUIT) {
		//if there are any window messages, process them
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			EntryApp->Update();
		}
	}

	return 0;
}