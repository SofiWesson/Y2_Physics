#include "MenuState.h"
#include "App.h"
#include "GameStateManager.h"

#include "Input.h"
#include <glm/ext.hpp>
#include <Gizmos.h>
#include "Renderer2D.h"
#include "Font.h"

MenuState::MenuState(App *a_app) : m_app(a_app)
{
	
}

MenuState::~MenuState()
{
}

bool MenuState::startup()
{
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
		m_app->GetGSM()->PopState();
		m_app->GetGSM()->PushState("Play");
	}
}

void MenuState::draw()
{
	m_app->Get2DRenderer()->drawText(m_app->GetFont(), "Menu", 10, 30);
}
