#include "MenuState.h"
#include "App.h"
#include "GameStateManager.h"
#include "PlayState.h"

#include <glm/ext.hpp>
#include <Gizmos.h>
#include "Font.h"
#include <Input.h>
#include <glm/glm.hpp>

MenuState::MenuState(App *a_app) : m_app(a_app)
{
	
}

MenuState::~MenuState()
{
}

bool MenuState::startup()
{
	m_GSM = m_app->GetGSM();

	return true;
}

void MenuState::shutdown()
{
}

void MenuState::update(float dt)
{
	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed(aie::INPUT_KEY_1))
	{
		// instantiate Play state
		m_GSM->SetState("Play", new PlayState(m_app));
		m_app->GetGSM()->PopState();
		m_app->GetGSM()->PushState("Play");
	}
	else if (input->wasKeyPressed(aie::INPUT_KEY_2))
	{
		m_app->GetGSM()->PopState();
		m_app->GetGSM()->PushState("Controls");
	}
}

void MenuState::draw()
{
	m_app->clearScreen();
	
	// begin drawing sprites
	m_app->Get2DRenderer()->begin();

	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Menu", 10, 30);

	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Press 1 to Play", 10, m_app->getWindowHeight() - 30);
	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Press 2 for Controls", 10, m_app->getWindowHeight() - 70);

	m_app->Get2DRenderer()->end();
}

void MenuState::SetGSM(GameStateManager* a_GSM)
{
	m_GSM = a_GSM;
}
