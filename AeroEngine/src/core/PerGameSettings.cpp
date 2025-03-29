#include "AEPCH.h"

PerGameSettings* PerGameSettings::pgs_inst;

PerGameSettings::PerGameSettings()
{
	pgs_inst = this;
	wcscpy_s(pgs_inst->m_GameName, L"undefined");
	wcscpy_s(pgs_inst->m_ShortName, L"undefined");
	wcscpy_s(pgs_inst->m_BootTime, Time::GetDateTimeString(TRUE).c_str());

}

PerGameSettings::~PerGameSettings()
{
}
