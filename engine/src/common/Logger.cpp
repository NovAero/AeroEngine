#include "AeroEngine.h"
#include <fstream>
#include <ShlObj.h>

Logger* Logger::log_inst;

Logger::Logger()
{
	log_inst = this;
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

	OutputDebugString(buff);

	std::wfstream outFile;

	outFile.open(std::wstring(LogDirectory() + L"/" + LogFile()), std::ios_base::app);

	MessageBox(0, LogDirectory().c_str(), 0, 0);
}

std::wstring Logger::LogDirectory()
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

std::wstring Logger::LogFile()
{
	WCHAR file[1024];
	wcscpy_s(file, PerGameSettings::GameName());
	//wcscat_s(file, PerGameSettings::BootTime());
	wcscat_s(file, L".log");
	return file;
}
