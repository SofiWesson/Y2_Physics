#pragma once

#include "GameState.h"
#include "App.h"
#include "GameStateManager.h"

class App;
class GameStateManager;

class MenuState : public GameState
{
public:
	MenuState(App* a_app);
	virtual ~MenuState();

	virtual bool startup();
	virtual void shutdown();

	void update(float dt);
	void draw();

	void SetGSM(GameStateManager* a_GSM);

protected:
	App* m_app;
	GameStateManager* m_GSM;
};