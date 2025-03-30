#include "AEPCH.h"
#include "AEWindow.h"

namespace Win32 {

	AEWindow::AEWindow(std::wstring className, std::wstring classTitle, HICON icon, int width, int height)
		: Win32::SubObject(className, classTitle, icon), m_Width(width), m_Height(height)
	{

	}

	AEWindow::~AEWindow()
	{

	}

	VOID AEWindow::Initialise()
	{
		hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, m_Class.c_str(), m_Title.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, m_Width, m_Height, nullptr, nullptr, HInstance(), (void*)this);
		
		if (!hWnd) {
			MessageBox(0, L"Failed to create window", 0, 0);
			PostQuitMessage(0);
		}

		ShowWindow(hWnd, SW_SHOW);
	}

	LRESULT AEWindow::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return CommonMessageHandler(hwnd, message, wParam, lParam);
	}
}