#include <Windows.h>

#define MAX_MAIN_STRING 256
#define HInstance() GetModuleHandle(NULL)

WCHAR WindowClass[MAX_MAIN_STRING];
WCHAR WindowTitle[MAX_MAIN_STRING];

int CALLBACK WinMain(HINSTANCE , HINSTANCE , LPSTR , INT ) {

	//Window class setup list
	wcscpy_s(WindowClass, TEXT("AeroRenderer"));
	wcscpy_s(WindowTitle, TEXT("AeroRenderer (AeRe) 1.0a"));

	int WindowWidth = 1280;
	int WindowHeight = 720;
	
	WNDCLASSEX wcex;

	//Size allocated for application
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	//Extra memory allocaiton
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	//Cursor and background
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

	//hIcon is top left of app window, hIxonSm is taskbar icon
	wcex.hIcon = LoadIcon(0, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(0, IDI_APPLICATION);
	
	//class name
	wcex.lpszClassName = WindowClass;

	//drop down menus - none (its a game!!)
	wcex.lpszMenuName = nullptr;

	//current instance of the window
	wcex.hInstance = HInstance();

	//Process for application - set to default for now
	wcex.lpfnWndProc = DefWindowProc;

	//End of class setup

	//Register class after setup
	RegisterClassEx(&wcex);

	//create and display window - reminder hWnd | handle (for) Window
	HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, WindowClass, WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, WindowWidth, WindowHeight, nullptr, nullptr, HInstance(), nullptr);

	if (!hWnd) {
		MessageBox(0, L"Failed to create window", 0, 0);
		return 0;
	}

	//Display window
	ShowWindow(hWnd, SW_SHOW);

	//Listen for messages

	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {

		//if there are any window messages, process them
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 1;
}