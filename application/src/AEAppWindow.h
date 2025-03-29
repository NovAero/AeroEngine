#pragma once

#include "platform/_Win32/IAEAppWindow.h"

class AEAppWindow : public IAEAppWindow {
public:
	AEAppWindow();

	~AEAppWindow();

public:

	VOID SetupPGS();

	VOID Initialise();

	VOID Update();
};