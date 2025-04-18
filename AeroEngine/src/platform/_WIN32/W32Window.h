#pragma once

#include "SubObject.h"
#include <Uxtheme.h>
#pragma comment (lib, "uxtheme.lib")

namespace Win32 {

	class AERO_API W32Window : public Win32::SubObject, public Win32::Caption {
	public:
		W32Window(WSTRING title, HICON icon, EWindowType type = RESIZEABLE);
		~W32Window();

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

		SIZE				m_size;

		RECT				m_windowRect;
		FLOAT				m_aspectRatio = 1.773f;
		EWindowType		m_type;

		BOOL				m_active;

	public:

		SIZE Size()			{ return m_size; }
		FLOAT AsRatio()		{ return m_aspectRatio;}
		BOOL Active()		{ return m_active; }

	public:

		VOID Size(SIZE size)			{ m_size = size; }
		VOID Size(INT cx, INT cy)		{ m_size.cx = cx; m_size.cy = cy; }
		VOID AsRatio(FLOAT w, FLOAT h)	{ m_aspectRatio = w/h; }
		VOID AsRatio(SIZE size)			{ m_aspectRatio = (float)size.cx/ (float)size.cy;}
		VOID AsRatio(FLOAT ratio)		{ m_aspectRatio = ratio;}
		VOID Active(BOOL active)		{ m_active = active; }
	};
}