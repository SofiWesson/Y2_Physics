#pragma once

#include "GameState.h"
#include "App.h"

class App;

class MenuState : public GameState
{
public:
	MenuState(App* a_app);
	virtual ~MenuState();

	virtual bool startup();
	virtual void shutdown();

	void update(float dt);
	void draw();

protected:
	App* m_app;

};