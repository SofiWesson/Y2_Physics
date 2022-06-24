#pragma once

#include "GameState.h"
#include "Application.h"
#include "App.h"

#include <map>
#include <list>
#include <functional>

class GameStateManager : public aie::Application
{
public:
	GameStateManager();
	virtual ~GameStateManager();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void SetState(const char* a_name, GameState* a_gameState);
	void PushState(const char* a_name);
	void PopState();

	GameState* GetGameState(const char* a_state);

protected:
	std::map<const char*, GameState*> m_states;
	std::list<GameState*> m_stack;
	std::list<std::function<void()>> m_commands;

};