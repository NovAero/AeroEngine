#pragma once

#ifdef BUILD_DLL
	#define AERO_API __declspec(dllexport)
#else
	#define AERO_API __declspec(dllimport)
#endif

#define SMALL_NUMBER (1.e-4f)
#define TINY_NUMBER (1.e-8f)

#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)

typedef std::wstring WSTRING;
typedef std::string STRING;