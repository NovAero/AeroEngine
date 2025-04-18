#pragma once

class AERO_API AeroEngine;

namespace Engine {

	enum EEngineMode : INT {
		NONE,
		DEBUG,
		RELEASE,
		EDITOR,
		SERVER
	};

	extern AeroEngine g_AeroEngine;

	VOID AERO_API SetMode(EEngineMode mode);
	EEngineMode AERO_API GetMode();

	WSTRING AERO_API EngineModeToString();

}

using namespace Engine;

class AERO_API AeroEngine {
public:

	AeroEngine();
	~AeroEngine();

	EEngineMode GetEngineMode();
	VOID SetEngineMode(EEngineMode mode);

private:

	EEngineMode m_engineMode;
};