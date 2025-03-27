#pragma once
#include "aeredeps.h"
#include <string>

using namespace Microsoft::WRL; //for ComPtr, memory management

class AeReAppBase {

	AeReAppBase();
	AeReAppBase(int height, int width, std::wstring windowName);

	
};