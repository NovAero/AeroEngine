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

		RECT				m_WindowRect;
		FLOAT				m_AspectRatio = 1.773f;
		AEWindowType		m_Type;

		BOOL				m_Active;

	public:

		SIZE Size()			{ return m_Size; }
		FLOAT AsRatio()		{ return m_AspectRatio;}
		BOOL Active()		{ return m_Active; }

	public:

		VOID Size(SIZE size)			{ m_Size = size; }
		VOID Size(INT cx, INT cy)		{ m_Size.cx = cx; m_Size.cy = cy; }
		VOID AsRatio(FLOAT w, FLOAT h)	{ m_AspectRatio = w/h; }
		VOID AsRatio(SIZE size)			{ m_AspectRatio = (float)size.cx/ (float)size.cy;}
		VOID AsRatio(FLOAT ratio)		{ m_AspectRatio = ratio;}
		VOID Active(BOOL active)		{ m_Active = active; }
	};
}