#pragma once
#include <string>

class AERO_API Logger {

private:
	static Logger* log_inst;

public:
	static Logger* Instance() { return log_inst; }

public:
	Logger();
	~Logger();

	static VOID PrintLog(const WCHAR* fmt, ...);

	static std::wstring LogDirectory();
	static std::wstring LogFile();

	static VOID PrintDebugSeperator();

	static BOOL IsMTailRunning();

	static VOID StartMTail();
};