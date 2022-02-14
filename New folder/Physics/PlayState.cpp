#include "PlayState.h"
#include "PhysicsApp.h"
#include "App.h"

#include <Input.h>
#include <glm/ext.hpp>

PlayState::PlayState(App* a_app) : m_app(a_app)
{
}

PlayState::~PlayState()
{
}

bool PlayState::startup()
{
	m_physApp = new PhysicsApp();
	m_physApp->startup(m_app);

	return true;
}

void PlayState::shutdown()
{
	m_physApp->shutdown();

	delete m_physApp;
	m_physApp = nullptr;
}

void PlayState::update(float dt)
{
	m_physApp->update(dt);

	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed(aie::INPUT_KEY_P))
		m_app->GetGSM()->PushState("Pause");
}

void PlayState::draw()
{
	m_physApp->draw();
}
