#include "GameStateManager.h"
#include "GameState.h"

#include <Gizmos.h>
#include <glm/ext.hpp>
#include "Texture.h"
#include "Font.h"

GameStateManager::GameStateManager()
{

}

GameStateManager::~GameStateManager()
{
	for (auto iter = m_states.begin(); iter != m_states.end(); iter++)
	{
		if (iter->second != nullptr)
		{
			iter->second->shutdown();
			delete iter->second;
		}
	}
}

bool GameStateManager::startup()
{
	return true;
}

void GameStateManager::shutdown()
{
}

void GameStateManager::update(float deltaTime)
{
	for (auto cmd : m_commands)
		cmd();
	m_commands.clear();

	m_stack.back()->update(deltaTime);
}

void GameStateManager::draw()
{
	for (auto state : m_stack)
		state->draw();
}

void GameStateManager::SetState(const char* a_name, GameState* a_gameState)
{
	m_commands.push_back([=]()
		{
			if (m_states[a_name] != nullptr)
			{
				m_states[a_name]->shutdown();
				delete m_states[a_name];
			}

			m_states[a_name] = a_gameState;

			if (m_states[a_name] != nullptr)
				m_states[a_name]->startup();
		});
}

void GameStateManager::PushState(const char* a_name)
{
	m_commands.push_back([=]()
		{
			m_stack.push_back(m_states[a_name]);
		});
}

void GameStateManager::PopState()
{
	m_commands.push_back([=]()
		{
			m_stack.pop_back();
		});
}