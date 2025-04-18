#include "AEPCH.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Time.h"

WSTRING AERO_API Time::GetTime(BOOL stripped)
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	std::wstringstream wss;
	wss << std::put_time(&ltm, L"%T");

	WSTRING timeString = wss.str();

	if (stripped) {
		WSTRING chars = L":";
		for (WCHAR c : chars) {
			timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
		}
	}

	return timeString;
}

WSTRING AERO_API Time::GetDate(BOOL stripped)
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	std::wstringstream wss;
	wss << std::put_time(&ltm, L"%d/%m/%y/");

	WSTRING timeString = wss.str();

	if (stripped) {
		WSTRING chars = L"/";
		for (WCHAR c : chars) {
			timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
		}
	}

	return timeString;
}

WSTRING AERO_API Time::GetDateTimeString(BOOL stripped)
{
	WSTRING timeString = GetDate(stripped) + L" " + GetTime(stripped);

	if (stripped) {
		WSTRING chars = L" ";
		for (WCHAR c : chars) {
			timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
		}
	}

	return timeString;
}
