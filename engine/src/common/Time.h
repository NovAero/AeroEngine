#pragma once
#include <string>

namespace Time {
	std::wstring AERO_API GetTime(BOOL stripped = FALSE);

	std::wstring AERO_API GetDate(BOOL stripped = FALSE);

	std::wstring AERO_API GetDateTimeString(BOOL stripped = FALSE);
}