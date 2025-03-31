#include "AEPCH.h"
#include "SplashScreen.h"

#include "platform/_WIN32/Win32Utils.h"

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
		PostMessage(m_SplashWindow->GetHandle(), WM_OUTPUTMESSAGE, (WPARAM)message, 0);
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
	switch (message)
	{
	case WM_PAINT:
	{
		HBITMAP hbitmap;
		HDC hdc, hwendc;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hwnd, &ps);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		Win32::Utils::AddBitmap(PerGameSettings::SplashURL(), hdc);

		if (Engine::GetMode() != Engine::EngineMode::RELEASE) {
			std::wstring engineModeText = Engine::EngineModeToString() + L" Mode";
			SetTextAlign(hdc, TA_RIGHT);
			TextOut(hdc, m_Width - 15, 15, engineModeText.c_str(), wcslen(engineModeText.c_str()));
		}

		SetTextAlign(hdc, TA_CENTER);

		TextOut(hdc, m_Width / 2, m_Height - 30, m_OutputMessage, wcslen(m_OutputMessage));
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_OUTPUTMESSAGE:
	{
		WCHAR* msg = (WCHAR*)wParam;
		wcscpy_s(m_OutputMessage, msg);
		RedrawWindow(GetHandle(), NULL, NULL, RDW_INVALIDATE);
		return 0;
	}
	}

	return CommonMessageHandler(hwnd, message, wParam, lParam);
}
