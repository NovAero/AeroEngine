#include "AEPCH.h"

PerGameSettings* PerGameSettings::s_PgsInst;

PerGameSettings::PerGameSettings()
{
	s_PgsInst = this;
	wcscpy_s(s_PgsInst->m_gameName, L"undefined");
	wcscpy_s(s_PgsInst->m_shortName, L"undefined");
	wcscpy_s(s_PgsInst->m_bootTime, Time::GetDateTimeString(TRUE).c_str());
	wcscpy_s(s_PgsInst->m_splashURL, L"AeroEngine\\content\\images\\AeroEngineSplash.bmp");
}

PerGameSettings::~PerGameSettings()
{
}