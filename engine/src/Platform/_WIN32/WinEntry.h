#include "AeroEngine.h"
#include "IAEAppWindow.h"

extern IAEAppWindow* EntryApplication();

INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {

	auto EntryAp = EntryApplication();

	EntryAp->Initialise();

	return 0;
}