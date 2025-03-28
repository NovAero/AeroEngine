#pragma once

#include "Platform/_Win32/IAEAppWindow.h"

class AEAppWindow : public IAEAppWindow {
public:
	AEAppWindow();

	~AEAppWindow();

public:

	VOID Initialise();
	VOID Update();
};