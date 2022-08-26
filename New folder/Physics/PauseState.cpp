#include "PauseState.h"
#include "PhysicsApp.h"
#include "App.h"

#include <glm/ext.hpp>
#include <Gizmos.h>
#include "Texture.h"
#include "Font.h"
#include <Input.h>
#include <glm/glm.hpp>

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

	// black screen tint
	m_app->Get2DRenderer()->setRenderColour(0, 0, 0, 0.5f);
	m_app->Get2DRenderer()->drawBox(m_app->getWindowWidth() / 2, m_app->getWindowHeight() / 2, m_app->getWindowWidth(), m_app->getWindowHeight());

	m_app->Get2DRenderer()->setRenderColour(1, 1, 1);

	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(), 
		m_pausedText, 
		m_app->getWindowWidth() / 2 - m_textWidth / 2, 
		m_app->getWindowHeight() / 2 - m_textHeight / 2);

	m_app->Get2DRenderer()->end();
}

void PauseState::LoadUI()
{
	m_textWidth = m_app->GetFont()->getStringWidth(m_pausedText);
	m_textHeight = m_app->GetFont()->getStringHeight(m_pausedText);
}
