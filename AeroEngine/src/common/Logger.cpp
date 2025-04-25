#include "AEPCH.h"
#include <fstream>
#include <ShlObj.h>
#include <cstdio>
#include <TlHelp32.h>

Logger* Logger::s_LogInst;

Logger::Logger()
{
	s_LogInst = this;
}

Logger::~Logger()
{

}

VOID Logger::PrintLog(const WCHAR* fmt, ...)
{
	WCHAR buff[4096];
	va_list args;

	va_start(args, fmt);
	vswprintf_s(buff, fmt, args);
	va_end(args);

	std::wfstream outFile;

	outFile.open(WSTRING(LogDirectory() + L"/" + LogFile()), std::ios_base::app);

	if (outFile.is_open()) {
		WSTRING s = buff;
		outFile << L"[" << Time::GetDateTimeString() << L"]  " << s;
		outFile.close();
		OutputDebugString(s.c_str());
	}
	else {
		MessageBox(NULL, L"Unable to open log file. &PrintLog", L"Log Error", MB_OK);
	}
}

WSTRING Logger::LogDirectory()
{
	WCHAR path[1024];
	WCHAR* AppDataLocal;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &AppDataLocal);
	wcscpy_s(path, AppDataLocal);
	wcscat_s(path, L"\\");
	wcscat_s(path, PerGameSettings::GameName());
	CreateDirectory(path, NULL);
	wcscat_s(path, L"\\Log");
	CreateDirectory(path, NULL);

	return path;
}

WSTRING Logger::LogFile()
{
	WCHAR file[1024];
	wcscpy_s(file, PerGameSettings::GameName());
	wcscat_s(file, PerGameSettings::BootTime());
	wcscat_s(file, L".log");
	return file;
}

VOID Logger::PrintDebugSeperator()
{
	WSTRING s = L"\n--------------------------------------------------------------";

#ifdef _DEBUG
	std::wfstream outFile;
	outFile.open(WSTRING(LogDirectory() + L"/" + LogFile(), std::ios_base::app));

	if (outFile.is_open()) {
		outFile << s;
		outFile.close();
	}
	else {
		MessageBox(NULL, L"Unable to open log file. PrintDebugSeparator", L"Log Error", MB_OK);
	}
#endif
}

BOOL Logger::IsMTailRunning()
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry)) {
		while (Process32Next(snapshot, &entry)) {
			if (!_wcsicmp(entry.szExeFile, L"mTAIL.exe")) {
				exists = true;
			}
		}
	}

	CloseHandle(snapshot);
	return exists;
}

VOID Logger::StartMTail()
{
	if (IsMTailRunning()) {
		Logger::PrintLog(L"--MTail failed to start - Already running\n");
		return;
	}

	Logger::PrintLog(L"--Starting MTail\n");

	WCHAR path[MAX_PATH] = { 0 };
	GetCurrentDirectoryW(MAX_PATH, path);
	WSTRING url = path + WSTRING(L"/mTAIL.exe");
	WSTRING params = L" \"" + LogDirectory() + L"/" + LogFile() + L"\" /start";
	ShellExecute(0, NULL, url.c_str(), params.c_str(), NULL, SW_SHOWDEFAULT);
}
