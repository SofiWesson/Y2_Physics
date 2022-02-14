#pragma once

#include "GameState.h"

class App;
class PhysicsApp;

class PlayState : public GameState
{
public:
	PlayState(App* a_app);
	virtual ~PlayState();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float dt);
	virtual void draw();

protected:
	App* m_app;
	PhysicsApp* m_physApp = nullptr;

};