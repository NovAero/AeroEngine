#pragma once

#include "Events.h"

#include <memory>
#include <string>

namespace AE::Game {
	class GameBase : public std::enable_shared_from_this<GameBase> {
	public:
		GameBase(const WSTRING name, INT width, INT height, bool vSync);
		virtual ~GameBase();

		/*
		*	Initialise the DirectX runtime
		*/
		virtual BOOL Initialise();
		/*
		*	Load content required for the game
		*/
		virtual BOOL LoadContent() = 0;
		/*
		*	Unload content required for the game that was loaded in LoadContent
		*/
		virtual VOID UploadContent() = 0;
		/*
		*	Destroy any resources that are used by the game
		*/
		virtual VOID Destroy() = 0;

	protected:
		friend class Simulation;
		/*
		*	Update game logic
		*/
		virtual VOID OnUpdate(UpdateEventArgs& e);
	};
}