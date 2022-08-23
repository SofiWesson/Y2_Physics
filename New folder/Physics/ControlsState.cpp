#include "ControlsState.h"
#include "App.h"
#include "GameStateManager.h"

#include "Input.h"
#include <glm/ext.hpp>
#include <Gizmos.h>
#include "Renderer2D.h"
#include "Font.h"

ControlsState::ControlsState(App* a_app) : m_app(a_app)
{

}

ControlsState::~ControlsState()
{
}

bool ControlsState::startup()
{
	return true;
}

void ControlsState::shutdown()
{
}

void ControlsState::update(float dt)
{
	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed(aie::INPUT_KEY_BACKSPACE))
	{
		m_app->GetGSM()->PopState();
		m_app->GetGSM()->PushState("Menu");
	}
}

void ControlsState::draw()
{
	m_app->clearScreen();

	// begin drawing sprites
	m_app->Get2DRenderer()->begin();

	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Controls", 10, 30);
	const char* goBacText = "Backspace to go Back";
	int goBackWidth = m_app->GetFont()->getStringWidth(goBacText);
	m_app->Get2DRenderer()->drawText(m_app->GetFont(), goBacText, m_app->getWindowWidth() - goBackWidth - 20, 30);

	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Controlling the cue ball - Hold down left mouse button and move in", 10, m_app->getWindowHeight() - 30);
	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "opposite direction you wish to shoot.", 10, m_app->getWindowHeight() - 60);
	
	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Pause - p", 10, m_app->getWindowHeight() - 120);
	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Backspace - Go back to menu", 10, m_app->getWindowHeight() - 160);
	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Esc - Exit game", 10, m_app->getWindowHeight() - 200);

	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Once a ball is sunk it will be displayed under the player that sunk it,", 10, m_app->getWindowHeight() - 260);
	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "even if the ball belongs to the other player. This is to show the", 10, m_app->getWindowHeight() - 300);
	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "players their faults.", 10, m_app->getWindowHeight() - 340);

	m_app->Get2DRenderer()->end();
}