#include "App.h"
#include "GameStateManager.h"
#include "Application.h"
#include "MenuState.h"
#include "PauseState.h"
#include "ControlsState.h"
#include "GameOverState.h"

#include <glm/ext.hpp>
#include <Gizmos.h>
#include "Texture.h"
#include "Font.h"
#include <Input.h>

App::App() : Application()
{
}

App::~App()
{
}

bool App::startup()
{
	// Increase the 2d line count to maximise the number of objects we can draw

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("./bin/font/consolas.ttf", 32);

	// ===================================GAME STATE MANAGER==============================
	m_GSM = new GameStateManager();

	m_GSM->SetState("Menu", new MenuState(this));
	m_GSM->SetState("Controls", new ControlsState(this));
	m_GSM->SetState("GameOver", new GameOverState(this));
	m_GSM->SetState("Pause", new PauseState(this));
	m_GSM->PushState("Menu");
	
	return true;
}

void App::shutdown()
{
	m_GSM->shutdown();
	delete m_GSM;
}

void App::update(float deltaTime)
{
	m_GSM->update(deltaTime);

	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void App::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));

	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 10);

	m_GSM->draw();

	// done drawing sprites
	m_2dRenderer->end();
}