#include "AEPCH.h"
#include "AEWindow.h"

#define DCX_UESTYLE 0x00010000

namespace Win32 {

	AEWindow::AEWindow(WSTRING title, HICON icon, AEWindowType type)
		: Win32::SubObject(title, title, icon), m_Type(type)
	{
		Size(DEFAULTWIDTH, DEFAULTHEIGHT);
	}

	AEWindow::~AEWindow()
	{

	}

	VOID AEWindow::Initialise()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		RECT R = { 0,0, Size().cx, Size().cy};

		AdjustWindowRect(&R, m_Type, false);

		m_Handle = CreateWindow(m_Class.c_str(), m_Title.c_str(),
			m_Type, ((desktop.right / 2) - (Size().cx / 2)), ((desktop.bottom / 2) - ( Size().cy / 2)), Size().cx, Size().cy,
			0, 0, HInstance(), (void*)this);
	}

	LRESULT AEWindow::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {

			case WM_NCCREATE:		 { OnNonClientCreate();									}	return TRUE;
			case WM_NCACTIVATE:		 { OnNonClientActivate(LOWORD(wParam) != WA_INACTIVE);  }	return TRUE;
			case WM_NCPAINT:		 { OnNonClientPaint((HRGN)wParam);						}	return FALSE;
			case WM_NCLBUTTONDOWN:   { OnNonClientLeftMouseButtonDown();					}	break;
			case WM_NCLBUTTONDBLCLK: { Win32::Utils::MaximiseWindow(Handle());				}   return 0;

			case WM_GETMINMAXINFO:   { OnGetMinMaxInfo((MINMAXINFO*)lParam);				}	return 0;
			case WM_EXITSIZEMOVE:	 { OnExitSizeMove();									}	break;
			case WM_TIMER:			 { RedrawWindow();										}	break;
			case WM_PAINT:			 { OnPaint();											}   break;
			case WM_DESTROY:		 { PostQuitMessage(0);									}	return 0;
		}

		return SubObject::MessageHandler(hwnd, message, wParam, lParam);
	}

	VOID AEWindow::RedrawWindow() {
		SetWindowPos(Handle(), 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED);

		SendMessage(Handle(), WM_PAINT, 0, 0);
	}

	VOID AEWindow::OnNonClientCreate() {

		SetTimer(Handle(), 1, 100, NULL);
		SetWindowTheme(Handle(),L"", L"");
		Win32::Utils::ModifyClassStyle(Handle(), 0, CS_DROPSHADOW);

		Win32::Caption::AddCaptionButton(new CaptionButton(L"X", CB_CLOSE));
		Win32::Caption::AddCaptionButton(new CaptionButton(L"🗖", CB_MAXIMISE));
		Win32::Caption::AddCaptionButton(new CaptionButton(L"🗕", CB_MINIMISE));
	}

	VOID AEWindow::OnNonClientActivate(BOOL active) {
		Active(active);
	}

	VOID AEWindow::OnNonClientPaint(HRGN region) {

		//start draw
		HDC hdc = GetDCEx(Handle(), region, DCX_WINDOW | DCX_INTERSECTRGN | DCX_UESTYLE);

		RECT rect;
		GetWindowRect(Handle(), &rect);

		SIZE size = SIZE{ rect.right - rect.left, rect.bottom - rect.top };

		HBITMAP hbaMem = CreateCompatibleBitmap(hdc, size.cx, size.cy);
		HANDLE hOld = SelectObject(hdc, hbaMem);

		//draw 
		RECT newRect = RECT{ 0, 0 ,size.cx, size.cy };

		HBRUSH brush = CreateSolidBrush(RGB(36, 36, 36));
		FillRect(hdc, &newRect, brush);

		if (Active() && !Win32::Utils::IsWindowFullscreen(Handle())) {
			brush = CreateSolidBrush(RGB(0,100,150));
			FrameRect(hdc, &newRect, brush );
		}

		PaintCaption(hdc);

		DeleteObject(brush);


		//end draw

		BitBlt(hdc, 0, 0, size.cx, size.cy, hdc, 0, 0, SRCCOPY);
		SelectObject(hdc, hOld);
		DeleteObject(hbaMem);

		ReleaseDC(Handle(), hdc);
	}

	VOID AEWindow::PaintCaption(HDC hdc) {

		RECT rect;
		GetWindowRect(Handle(), &rect);
		
		SIZE size = SIZE{ rect.right - rect.left, rect.bottom - rect.top };

		//Draw title text
		if (ShowTitle()) {

			rect = RECT{ 0,0,size.cx, 30 };

			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, Active() ? RGB(255, 255, 255) : RGB(92, 92, 92));

			DrawText(hdc, m_Title.c_str(), wcslen(m_Title.c_str()), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		}

		int spacing = 0;

		POINT pt;
		GetCursorPos(&pt);

		GetWindowRect(Handle(), &rect);

		pt.x -= rect.left;
		pt.y -= rect.top;

		//Draw all buttons
		for (auto button : Caption::CaptionButtons()) {

			spacing += button->Width;

			button->Rect = RECT{ size.cx - spacing, 0, size.cx - spacing + button->Width, 30 };

			if (button->Rect.left < pt.x && button->Rect.right > pt.x && button->Rect.top < pt.y && button->Rect.bottom > pt.y) {
				HBRUSH brush = CreateSolidBrush(RGB(96, 96, 96));
				
				FillRect(hdc, &button->Rect, brush);

				DeleteObject(brush);
			}

			if (button->Text.compare(L"🗖") == 0 && Win32::Utils::IsWindowFullscreen(Handle())) {
				button->Text = L"🗗";
			}
			else if (button->Text.compare(L"🗗") == 0 && !Win32::Utils::IsWindowFullscreen(Handle())) {
				button->Text = L"🗖";
			}

			DrawText(hdc, button->Text.c_str(), wcslen(button->Text.c_str()), &button->Rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}

	}

	VOID AEWindow::OnNonClientLeftMouseButtonDown() {


		POINT pt;
		GetCursorPos(&pt);

		RECT rect;
		GetWindowRect(Handle(), &rect);

		pt.x -= rect.left;
		pt.y -= rect.top;

		//Draw all buttons
		for (auto button : Caption::CaptionButtons()) {

			if (button->Rect.left < pt.x && button->Rect.right > pt.x && button->Rect.top < pt.y && button->Rect.bottom > pt.y) {
				
				switch (button->Command) {
					case CB_CLOSE:		{ SendMessage(Handle(), WM_CLOSE, 0, 0);		}	break;
					case CB_MINIMISE:	{ ShowWindow(Handle(), SW_MINIMIZE);			}	break;
					case CB_MAXIMISE:	{ Utils::MaximiseWindow(Handle());				}	break;
				}
			}
		}
	}

	VOID AEWindow::OnGetMinMaxInfo(MINMAXINFO* minmax) {
		RECT WorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);

		minmax->ptMaxSize.x = (WorkArea.right - WorkArea.left);
		minmax->ptMaxSize.y = (WorkArea.bottom - WorkArea.top);
		minmax->ptMaxPosition.x = WorkArea.left;
		minmax->ptMaxPosition.y = WorkArea.top;
		minmax->ptMinTrackSize.x = 400;
		minmax->ptMinTrackSize.y = 300;
	}

	VOID AEWindow::OnExitSizeMove() {
		RECT rect;
		GetWindowRect(Handle(), &rect);
		RECT workArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
		if (rect.top < workArea.top + 10 && !Utils::IsWindowFullscreen(Handle())) {
			Utils::MaximiseWindow(Handle());
		}

	}

	VOID AEWindow::OnPaint() {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(Handle(), &ps);

		RECT rc;
		GetClientRect(Handle(), &rc);

		HBRUSH brush = CreateSolidBrush(RGB(36, 36, 36));

		FillRect(hdc, &rc, brush);

		DeleteObject(brush);

		EndPaint(Handle(), &ps);
	}
}