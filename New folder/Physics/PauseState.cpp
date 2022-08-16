#include "PauseState.h"
#include "PhysicsApp.h"
#include "App.h"

#include <Input.h>
#include <glm/ext.hpp>

PauseState::PauseState(App* a_app) : m_app(a_app)
{
}

PauseState::~PauseState()
{
}

bool PauseState::startup()
{
	return true;
}

void PauseState::shutdown()
{
}

void PauseState::update(float dt)
{
	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed(aie::INPUT_KEY_P))
		m_app->GetGSM()->PopState();
}

void PauseState::draw()
{
	// begin drawing sprites
	m_app->Get2DRenderer()->begin();

	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Game Paused", 10, 30);

	m_app->Get2DRenderer()->end();
}
