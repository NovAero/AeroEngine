#pragma once 

class AERO_API PerGameSettings {

private:
	static PerGameSettings* pgs_inst;

	static PerGameSettings* Instance() { return pgs_inst; }

public:

	PerGameSettings();
	~PerGameSettings();


public:
	static WCHAR* GameName() { return pgs_inst->m_GameName; }
	static VOID SetGameName(UINT id) { LoadString(HInstance(), id, pgs_inst->m_GameName, MAX_NAME_STRING); }

	static WCHAR* ShortName() { return pgs_inst->m_ShortName; }
	static VOID SetShortName(UINT id) { LoadString(HInstance(), id, pgs_inst->m_ShortName, MAX_NAME_STRING); }

	static HICON MainIcon() { return pgs_inst->m_MainIcon; }
	static VOID SetMainIcon(UINT id) { LoadIcon(HInstance(), MAKEINTRESOURCE(id)); }

private:

	WCHAR m_GameName[MAX_NAME_STRING];
	WCHAR m_ShortName[MAX_NAME_STRING];
	HICON m_MainIcon;
	WCHAR m_BootTime[MAX_NAME_STRING];

};