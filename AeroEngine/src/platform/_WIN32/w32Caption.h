#pragma once

#define			CB_CLOSE		 0
#define			CB_MINIMISE		 1
#define			CB_MAXIMISE		 2

namespace Win32 {
	class AERO_API Caption {

	public:
		struct CaptionButton {

			WSTRING		Text = L"X";
			INT			Command = -1;
			int			Width = 50;
			RECT		Rect = RECT{ 0 };

			CaptionButton(WSTRING text, INT command, INT width = 50) {
				Text = text;
				Command = command;
				Width = width;
			}
		};

	public:
		VOID AddCaptionButton(CaptionButton* button);


	private:
		std::list<CaptionButton*>		m_captionButtons;

		BOOL							m_showTitle = TRUE;

	public:

		BOOL ShowTitle()							{ return m_showTitle; }
		std::list<CaptionButton*> CaptionButtons()  { return m_captionButtons; }

	public:
		VOID ShowTitle(BOOL show)					{ m_showTitle = show; }

	};
}