#pragma once

#include "App.h"
#include "Renderer2D.h"
#include <Input.h>
#include <list>
#include <iostream>

#include "PhysicsScene.h"
#include "GameStateManager.h"

class App;
class Circle;
class Plane;
class Player;
class Box;
class Ball;
class GameOverState;

class PhysicsApp
{
public:

	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup(App* a_app);
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec2 ScreenToWorld(glm::vec2 a_screenPos);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	App* m_app;
	PhysicsScene* m_physicsScene;
	GameOverState* m_gameOverState;

	float m_timer = 0.f;
	float m_timerReset = 0.1f;

	Player* m_player;
	Circle* m_ball;

	Ball* m_cue;
	glm::vec2 m_cueForceVectorStart = glm::vec2(0, 0);
	glm::vec2 m_cueForce = glm::vec2(0, 0);
	std::list<Ball*> m_balls;
	bool m_ballsStatic = true;

	BallType m_player1 = NUL;
	BallType m_player2 = NUL;

	std::list<Ball*> m_p1Sunk;
	std::list<Ball*> m_p2Sunk;

	bool m_firstBallHasBeenSunk = false;
	bool m_hasBallBeenSunk = false;
	bool m_isPlayer1Turn = true;
	bool m_inPlay = false;
	bool m_inPlayLastFrame = false;

	const float m_extents = 100;
	const float m_aspectRatio = 16.f / 9.f;

	// ============================================================ UI ============================================================
	// UI Text
	const char* m_player1Text = "Player 1";
	const char* m_player2Text = "Player 2";
	const char* m_colourText = "Colour";
	const char* m_noBallsSunkText = "No Balls Sunk Yet";
	const char* m_yellowText = "Yellow";
	const char* m_redText = "Red";
	const char* m_waitText = "Wait";
	const char* m_goText = "Go";

	const char* m_player1ColourText = "";
	const char* m_player2ColourText = "";
	const char* m_tableActivityText = "";

	// UI Size
	float m_textHeight;

	float m_player1TextWidth;
	float m_player2TextWidth;
	float m_colourTextWidth;
	float m_noBallsTextWidth;
	float m_yellowTextWidth;
	float m_redTextWidth;
	float m_waitTextWidth;
	float m_goTextWidth;

	float m_player1ColourTextWidth = 0.f;
	float m_player2ColourTextWidth = 0.f;
	float m_tableActivityTextWidth = 0.f;
	// ============================================================ UI ============================================================

public:
	Circle* CreateCircle(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec4 a_colour, glm::vec2 a_force, bool a_isKinematic, bool a_isTrigger, bool a_canCollide);
	Ball* CreateBall(BallType a_ballType, glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec2 a_force, bool a_isKinematic, bool a_isTrigger, bool a_canCollide);
	Plane* CreatePlane(glm::vec2 a_normal, float a_distToOrigin, glm::vec4 a_colour);
	Box* CreateBox(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour, glm::vec2 a_force, bool a_isKinematic, bool a_isTrigger, bool a_canCollide);
	Player* CreatePlayer(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec4 a_colour);
	Player* CreatePlayer(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour);
	void CreateSpring(int a_amount);
	void CreateTable();
	void RackBalls();
	void HitCueBall(aie::Input* a_input);
	void SoftbodyTest();

	void LoadUI();
	void EightBallWinConditions();
	void BallSunk(Ball* a_ball);

	bool m_keyPressed = false;
};