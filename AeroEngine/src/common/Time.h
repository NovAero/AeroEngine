#pragma once
#include <string>

namespace Time {
	WSTRING AERO_API GetTime(BOOL stripped = FALSE);

	WSTRING AERO_API GetDate(BOOL stripped = FALSE);

	WSTRING AERO_API GetDateTimeString(BOOL stripped = FALSE);
}