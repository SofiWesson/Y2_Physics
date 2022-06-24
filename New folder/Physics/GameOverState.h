#pragma once
#include "GameState.h"
#include "App.h"

class App;

class GameOverState : public GameState
{
public:
	GameOverState(App* a_app);
	~GameOverState();
	bool startup();
	void shutdown();

	void update(float dt);
	void draw();

	void SetPlayWon(const char* a_player);

private:
	App* m_app;
	const char* m_player = "";
	const char* m_winText = " WINS!!!";
};