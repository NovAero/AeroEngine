#pragma once

#include "core/CoreDefinitions.h"

#include "engine/AeroEngine.h"

#include "common/Logger.h"
#include "common/Time.h"
#include "core/PerGameSettings.h"

#include "core/Matrix3x3.h"

#ifdef WIN32

#pragma warning (disable : 4251)

#include "platform/_WIN32/Win32Utils.h"
#include "platform/_WIN32/SubObject.h"
#include "platform/_WIN32/w32Caption.h"
#include "platform/_WIN32/W32Window.h"
#include "platform/_WIN32/IAEApplication.h"

#endif