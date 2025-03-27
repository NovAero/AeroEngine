#pragma once
#include "aeredeps.h"
#include <string>

using namespace Microsoft::WRL; //for ComPtr, memory management

class AEAppWindow {
public:
	AEAppWindow();
	AEAppWindow(int height, int width, std::wstring windowName);

	int Initialise();
	bool Display();
	bool MessageLoop();
	void Exit();

private:

	WNDCLASSEX	wcex;

	HWND		hWnd; //Window handle
	MSG			msg = { 0 }; //Message listener

	WCHAR		WindowClass[MAX_NAME_STRING];
	WCHAR		WindowTitle[MAX_NAME_STRING];

	int			WindowWidth;
	int			WindowHeight;

	HICON		hIcon;
};