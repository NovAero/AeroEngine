#pragma once

#include "SubObject.h"

namespace Win32 {

	class AERO_API AEWindow : public Win32::SubObject {
	public:
		AEWindow(std::wstring className, std::wstring classTitle, HICON icon, int width, int height);
		~AEWindow();

		virtual VOID Initialise() override;

		virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;


	private:

		HWND		hWnd; //Window handle

		INT			m_Width;
		INT			m_Height;

	};
}