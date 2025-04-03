#pragma once

#include "SubObject.h"
#include <Uxtheme.h>
#pragma comment (lib, "uxtheme.lib")

namespace Win32 {

	class AERO_API AEWindow : public Win32::SubObject, public Win32::Caption {
	public:
		AEWindow(WSTRING title, HICON icon, AEWindowType type = RESIZEABLE);
		~AEWindow();

		virtual VOID Initialise() override;

		virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

		VOID RedrawWindow();

		VOID OnNonClientCreate();
		VOID OnNonClientActivate(BOOL active);
		VOID OnNonClientPaint(HRGN region);
		VOID OnNonClientLeftMouseButtonDown();

		VOID OnGetMinMaxInfo(MINMAXINFO* minmax);

		VOID OnExitSizeMove();

		VOID OnPaint();

		VOID PaintCaption(HDC hdc);

	protected:

		SIZE				m_Size;
		AEWindowType		m_Type;

		BOOL				m_Active;

	public:

		SIZE Size()			{ return m_Size; }
		BOOL Active()		{ return m_Active; }

	public:

		VOID Size(SIZE size)			{ m_Size = size; }
		VOID Size(int cx, int cy)		{ m_Size.cx = cx; m_Size.cy = cy; }
		VOID Active(BOOL active)		{ m_Active = active; }
	};
}