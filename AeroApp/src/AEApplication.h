#pragma once

#include "engine/Simulation.h"

class AEApplication : public AE::Simulation{
public:
	AEApplication();

	~AEApplication();

public:

	VOID SetupPGS();

	VOID Initialise();

	VOID Update();
};