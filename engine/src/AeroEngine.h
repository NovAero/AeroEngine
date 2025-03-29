#pragma once

#ifdef WIN32

#include <Windows.h>

#endif

#ifdef BUILD_DLL
	#define AERO_API __declspec(dllexport)
#else
	#define AERO_API __declspec(dllimport)
#endif

#define HInstance() GetModuleHandle(NULL)
#define MAX_NAME_STRING 256

#include "common/Logger.h"
#include "core/PerGameSettings.h"