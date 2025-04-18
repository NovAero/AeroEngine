#include "AEPCH.h"
#include "SplashScreen.h"

namespace SplashScreen {
	
#define WM_OUTPUTMESSAGE (WM_USER + 0x0001)

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
		PostMessage(m_SplashWindow->Handle(), WM_OUTPUTMESSAGE, (WPARAM)message, 0);
	}
}

SplashWindow::SplashWindow()
	: Win32::W32Window(L"SplashScreen", NULL, Win32::EWindowType::POPUP)
{
	wcscpy_s(m_outputMessage, L"SplashScreen starting...");

	Win32::W32Window::RegisterNewClass();
	Size(500, 600);
	Win32::W32Window::Initialise();

}

SplashWindow::~SplashWindow()
{
}

LRESULT SplashWindow::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		HBITMAP hbitmap;
		HDC hdc;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hwnd, &ps);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		Win32::Utils::AddBitmap(PerGameSettings::SplashURL(), hdc);

		if (Engine::GetMode() != Engine::EEngineMode::RELEASE) {
			WSTRING engineModeText = Engine::EngineModeToString() + L" Mode";
			SetTextAlign(hdc, TA_RIGHT);
			TextOut(hdc, Size().cx - 15, 15, engineModeText.c_str(), wcslen(engineModeText.c_str()));
		}

		SetTextAlign(hdc, TA_CENTER);

		TextOut(hdc, Size().cx / 2, Size().cy - 30, m_outputMessage, wcslen(m_outputMessage));
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_OUTPUTMESSAGE:
	{
		WCHAR* msg = (WCHAR*)wParam;
		wcscpy_s(m_outputMessage, msg);
		RedrawWindow();
		return 0;
	}
	}

	return W32Window::MessageHandler(hwnd, message, wParam, lParam);
}
