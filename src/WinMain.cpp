#include "pch.h"
#include "AeReAppBase.h"

int CALLBACK WinMain(HINSTANCE , HINSTANCE , LPSTR , INT ) {
	
	AeReAppBase app;

	if (app.OnStart()) {
		while (app.OnUpdate(0.f)) {
			app.Draw();
		}
	}
	app.OnExit();

	return 1;
}
