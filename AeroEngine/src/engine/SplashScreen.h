#pragma once

namespace SplashScreen {

	VOID AERO_API Open();
	VOID AERO_API Close();
	VOID AERO_API AddMessage(const WCHAR* message);

}

class AERO_API SplashWindow : public Win32::AEWindow {
public:
	SplashWindow();
	~SplashWindow();

	virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;


private:
	WCHAR m_OutputMessage[MAX_NAME_STRING];
};