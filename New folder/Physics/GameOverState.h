#pragma once
#include "GameState.h"
#include "App.h"

class App;
class PhysicsApp;

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
	void SetPhysicsApp(PhysicsApp* a_physicsApp) { m_physicsApp = a_physicsApp; }

private:
	App* m_app;
	PhysicsApp* m_physicsApp;

	const char* m_player = "";
	const char* m_winText = " WINS!!!";
	const char* m_backText = "Backspace to go Back";

	float m_backTextWidth;
};