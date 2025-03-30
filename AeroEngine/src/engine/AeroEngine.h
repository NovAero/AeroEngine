#pragma once
#include <string>

class AERO_API AeroEngine;

namespace Engine {

	enum EngineMode : INT {
		NONE,
		DEBUG,
		RELEASE,
		EDITOR,
		SERVER
	};

	extern AeroEngine g_AeroEngine;

	VOID AERO_API SetMode(EngineMode mode);
	EngineMode AERO_API GetMode();

	std::wstring AERO_API EngineModeToString();

}

using namespace Engine;

class AERO_API AeroEngine {
public:

	AeroEngine();
	~AeroEngine();

	EngineMode GetEngineMode();
	VOID SetEngineMode(EngineMode mode);

private:

	EngineMode m_EngineMode;
};