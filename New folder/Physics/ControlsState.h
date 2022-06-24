#pragma once
#include "GameState.h"
#include "App.h"

class App;

class ControlsState : public GameState
{
public:
	ControlsState(App* a_app);
	~ControlsState();
	bool startup();
	void shutdown();

	void update(float dt);
	void draw();

private:
	App* m_app;
};