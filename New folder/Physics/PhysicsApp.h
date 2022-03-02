#pragma once

#include "App.h"
#include "Renderer2D.h"
#include <Input.h>
#include <list>

#include "PhysicsScene.h"
#include "GameStateManager.h"

class App;
class Circle;
class Plane;
class Player;
class Box;
class Ball;

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

	float m_timer = 0.f;
	float m_timerReset = 0.1f;

	//Application2D* m_app;
	Player* m_player;
	Circle* m_ball;

	Ball* m_cue;
	glm::vec2 m_cueForceVectorStart = glm::vec2(0, 0);
	glm::vec2 m_cueForce = glm::vec2(0, 0);
	std::list<Ball*> m_balls;
	bool m_ballsStatic = true;

	BallType m_player1 = NUL;
	BallType m_player2 = NUL;

	bool m_firstBallHasBeenSunk = false;
	bool m_hasBallBeenSunk = false;
	bool m_isPlayer1Turn = true;

	const char* m_player1Colour = "";
	const char* m_player2Colour = "";

	const float m_extents = 100;
	const float m_aspectRatio = 16.f / 9.f;

//====================
public:
	Circle* CreateCircle(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec4 a_colour, glm::vec2 a_force, bool a_isKinematic, bool a_isTrigger, bool a_canCollide);
	Ball* CreateBall(BallType a_ballType, glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec2 a_force, bool a_isKinematic, bool a_isTrigger, bool a_canCollide);
	Plane* CreatePlane(glm::vec2 a_normal, float a_distToOrigin, glm::vec4 a_colour);
	Box* CreateBox(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour, glm::vec2 a_force, bool a_isKinematic, bool a_isTrigger, bool a_canCollide);
	Player* CreatePlayer(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec4 a_colour);
	Player* CreatePlayer(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour);
	void CreateSpring(int a_amount); // make proper constructor later
	void CreateTable();
	//void CreateEdges();
	void RackBalls();
	void HitCueBall(aie::Input* a_input);
	void SoftbodyTest();
	void MouseInputTest(aie::Input* a_input);
	void ObjectTest();

	bool m_keyPressed = false;
};