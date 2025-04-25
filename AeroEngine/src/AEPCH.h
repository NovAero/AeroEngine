#pragma once

#ifdef WIN32
#include <Windows.h>
#endif

#include <string>
#include <list>
#include <algorithm>
#include <cassert>
#include <chrono>

#include "core/Core.h"

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL			((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN		((D3D12_GPU_VIRTUAL_ADDRESS)-1)	