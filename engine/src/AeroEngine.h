#pragma once

#ifdef WIN32

#include <Windows.h>

#endif

#ifdef BUILD_DLL
	#define AERO_API __declspec(dllexport)
#else
	#define AERO_API __declspec(dllimport)
#endif