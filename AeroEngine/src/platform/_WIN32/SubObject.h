#pragma once

namespace Win32 {

	class AERO_API SubObject {
	public:
		SubObject(WSTRING className, WSTRING classTitle, HICON icon);
		~SubObject();

		virtual VOID RegisterNewClass();
		virtual VOID Initialise() = 0;

		static	LRESULT CALLBACK SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static	LRESULT			 AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual	LRESULT			 MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
						
	protected:
		WSTRING			m_class;
		WSTRING			m_title;

		HICON			m_hIcon;

		HWND			m_handle;


	public:
		HWND Handle() { return m_handle; }
	
	public:
		VOID Handle(HWND hwnd) { m_handle = hwnd; }
	};
}