#include "AEPCH.h"
#include "IAEApplication.h"
#include "common/CmdLineArgs.h"
#include "engine/SplashScreen.h"
#include "engine/Simulation.h"

//CoreMinimal disables warning C4251 - can be found through AEPCH.h -> Core.h -> CoreMinimal.h

/*----------------------------------------------*/
/* Author: NovAero								*/
/* License: MIT License							*/
/*----------------------------------------------*/

extern Win32::IAEApplication* EntryApplication();

INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {

	auto EntryApp = EntryApplication();

	PerGameSettings settings;
	EntryApp->SetupPGS();

	CmdLineArgs::ReadArgs();

	Logger logger;

	EntryApp->Register();
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

	EntryApp->Exit();

	return 0;
}