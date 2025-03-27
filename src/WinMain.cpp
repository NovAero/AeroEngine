#include "pch.h"
#include "AeReAppBase.h"

/*----------------------------------------------*/
/* Author: NovAero								*/
/* License: MIT License							*/
/*----------------------------------------------*/

int CALLBACK WinMain(HINSTANCE , HINSTANCE , LPSTR , INT ) {
	
	AeReAppBase app;

	if (app.Display()) {
		app.MessageLoop();
	}
	app.Exit();

	return 1;
}
