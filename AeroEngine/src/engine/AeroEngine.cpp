#include "AEPCH.h"

namespace Engine {
	AeroEngine g_AeroEngine;
}

AeroEngine::AeroEngine()
{
#ifdef _DEBUG
	m_EngineMode = EngineMode::DEBUG;
#else
	m_EngineMode = EngineMode::RELEASE;
#endif

}

AeroEngine::~AeroEngine()
{
}
