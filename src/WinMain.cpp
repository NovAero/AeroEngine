#include "pch.h"
#include "AEAppWindow.h"

/*----------------------------------------------*/
/* Author: NovAero								*/
/* License: MIT License							*/
/*----------------------------------------------*/

int CALLBACK WinMain(HINSTANCE , HINSTANCE , LPSTR , INT ) {
	
	AEAppWindow app;

	if (app.Display()) {
		app.MessageLoop();
	}
	app.Exit();

	return 1;
}
