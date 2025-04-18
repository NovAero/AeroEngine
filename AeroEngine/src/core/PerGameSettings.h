#pragma once 

class AERO_API PerGameSettings {

private:
	static PerGameSettings* s_PgsInst;

	static PerGameSettings* PerGameSettings_Inst() { return s_PgsInst; }

public:

	PerGameSettings();
	~PerGameSettings();

private:

	WCHAR m_gameName[MAX_NAME_STRING];
	WCHAR m_shortName[MAX_NAME_STRING];
	HICON m_mainIcon;
	WCHAR m_bootTime[MAX_NAME_STRING];
	WCHAR m_splashURL[MAX_NAME_STRING];

public:

	static WCHAR* GameName() { return s_PgsInst->m_gameName; }
	static VOID SetGameName(UINT id) { LoadString(HInstance(), id, s_PgsInst->m_gameName, MAX_NAME_STRING); }

	static WCHAR* ShortName() { return s_PgsInst->m_shortName; }
	static VOID SetShortName(UINT id) { LoadString(HInstance(), id, s_PgsInst->m_shortName, MAX_NAME_STRING); }

	static HICON MainIcon() { return s_PgsInst->m_mainIcon; }
	static VOID SetMainIcon(UINT id) { LoadIcon(HInstance(), MAKEINTRESOURCE(id)); }
	
	static WCHAR* BootTime() { return s_PgsInst->m_bootTime; }

	static WCHAR* SplashURL() { return s_PgsInst->m_splashURL; }
	static VOID SetSplashURL(UINT id) { LoadString(HInstance(), id, s_PgsInst->m_splashURL, MAX_NAME_STRING); }

};