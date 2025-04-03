#include "AEPCH.h"

namespace Engine {
	AeroEngine g_AeroEngine;
	VOID SetMode(EngineMode mode)
	{
		g_AeroEngine.SetEngineMode(mode);
	}

	EngineMode GetMode()
	{
		return g_AeroEngine.GetEngineMode();
	}

	WSTRING AERO_API EngineModeToString()
	{
		switch (g_AeroEngine.GetEngineMode())
		{
		case Engine::DEBUG:		return L"Debug";
		case Engine::RELEASE:	return L"Release";
		case Engine::EDITOR:	return L"Editor";
		case Engine::SERVER:	return L"Server";
		default:	return L"None";
		}
	}
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

EngineMode AeroEngine::GetEngineMode()
{
	return m_EngineMode;
}

VOID AeroEngine::SetEngineMode(EngineMode mode)
{
	m_EngineMode = mode;
}
