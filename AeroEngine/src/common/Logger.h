#pragma once
#include <string>

class AERO_API Logger {

private:
	static Logger* s_LogInst;

public:
	static Logger* Instance() { return s_LogInst; }

public:
	Logger();
	~Logger();

	static VOID PrintLog(const WCHAR* fmt, ...);

	static WSTRING LogDirectory();
	static WSTRING LogFile();

	static VOID PrintDebugSeperator();

	static BOOL IsMTailRunning();

	static VOID StartMTail();
};