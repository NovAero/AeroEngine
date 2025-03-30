#include "AEPCH.h"
#include "SplashScreen.h"

namespace SplashScreen {

	SplashWindow* m_SplashWindow;

	VOID Open()
	{
		if (m_SplashWindow) {
			return;
		}
		m_SplashWindow = new SplashWindow();
	}

	VOID Close()
	{
		return VOID AERO_API();
	}

	VOID AddMessage(const WCHAR* message)
	{
		return VOID AERO_API();
	}
}
SplashWindow::SplashWindow()
	: Win32::AEWindow(L"SplashScreen", L"SplashScreen", NULL, 500, 600)
{
	wcscpy_s(m_OutputMessage, L"SplashScreen starting...");
	Win32::AEWindow::RegisterNewClass();
	Win32::AEWindow::Initialise();
}

SplashWindow::~SplashWindow()
{
}

LRESULT SplashWindow::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CommonMessageHandler(hwnd, message, wParam, lParam);
}
