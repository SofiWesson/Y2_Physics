#pragma once

#include "GameState.h"

class App;
class PhysicsApp;

class PauseState : public GameState
{
public:
	PauseState(App* a_app);
	virtual ~PauseState();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float dt);
	virtual void draw();

protected:
	App* m_app;
	PhysicsApp* m_physApp;

};