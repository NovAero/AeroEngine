#pragma once

#include "platform/_Win32/IAEApplication.h"

class AEApplication : public IAEApplication {
public:
	AEApplication();

	~AEAppWindow();

public:

	VOID SetupPGS();

	VOID Initialise();

	VOID Update();
};