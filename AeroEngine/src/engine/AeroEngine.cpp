#include "AEPCH.h"

namespace Engine {
	AeroEngine g_AeroEngine;
	VOID SetMode(EEngineMode mode)
	{
		g_AeroEngine.SetEngineMode(mode);
	}

	EEngineMode GetMode()
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
	m_engineMode = EEngineMode::DEBUG;
#else
	m_EngineMode = EngineMode::RELEASE;
#endif

}

AeroEngine::~AeroEngine()
{
}

EEngineMode AeroEngine::GetEngineMode()
{
	return m_engineMode;
}

VOID AeroEngine::SetEngineMode(EEngineMode mode)
{
	m_engineMode = mode;
}
