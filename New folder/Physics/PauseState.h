#pragma once

#include "GameState.h"

class App;

class PauseState : public GameState
{
public:
	PauseState(App* a_app);
	virtual ~PauseState();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float dt);
	virtual void draw();

	void LoadUI();

protected:
	App* m_app;

private:
	float m_textWidth;
	float m_textHeight;

	const char* m_pausedText = "Game Paused";

};