#pragma once

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
}

using namespace Engine;

class AERO_API AeroEngine {
public:

	AeroEngine();
	~AeroEngine();

private:

	EngineMode m_EngineMode;
};