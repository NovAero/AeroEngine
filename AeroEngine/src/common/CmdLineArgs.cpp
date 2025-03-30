#include "AEPCH.h"
#include "CmdLineArgs.h"
#include <algorithm>

VOID CmdLineArgs::ReadArgs()
{
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	for (int i = 1; i < argc; ++i) {
		std::wstring key = argv[i];

		if (key[0] == '-') {
			key.erase(0, 1);
			
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);

			ReadArgument(key.c_str());
		}
	}


}

VOID CmdLineArgs::ReadArgument(const WCHAR* argument)
{

	if (wcscmp(argument, L"mtail") == 0) {
		Logger::StartMTail();
	}

}
