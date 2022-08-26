#include "GameOverState.h"
#include "App.h"
#include "GameStateManager.h"
#include "PhysicsApp.h"
#include "Renderer2D.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <Gizmos.h>
#include "Texture.h"
#include "Font.h"
#include <Input.h>
#include <cstring>

GameOverState::GameOverState(App* a_app) : m_app(a_app)
{

}

GameOverState::~GameOverState()
{

}

bool GameOverState::startup()
{
	return false;
}

void GameOverState::shutdown()
{
	
}

void GameOverState::update(float dt)
{
	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed(aie::INPUT_KEY_BACKSPACE))
	{
		m_physicsApp->ShutDownGame(m_app->GetGSM()->GetNumOfStates());
	}
}

void GameOverState::draw()
{
	m_app->clearScreen();

	// begin drawing sprites
	m_app->Get2DRenderer()->begin();

	char* playerWinText = new char[32];
	std::strcpy(playerWinText, m_player);
	std::strcat(playerWinText, m_winText);
	float playerWinTextWidth = m_app->GetFont()->getStringWidth(playerWinText);

	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		playerWinText,
		m_app->getWindowWidth() / 2 - playerWinTextWidth / 2,
		m_app->getWindowHeight() / 2);

	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_backText,
		m_app->getWindowWidth() / 2 - m_backTextWidth / 2,
		30);

	m_app->Get2DRenderer()->end();
}

void GameOverState::LoadUI()
{
	m_backTextWidth = m_app->GetFont()->getStringWidth(m_backText);
}

void GameOverState::SetPlayWon(const char* a_player)
{
	m_player = a_player;
}