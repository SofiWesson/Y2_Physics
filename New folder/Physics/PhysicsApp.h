#pragma once

#include "App.h"
#include "Renderer2D.h"
#include <Input.h>

#include "PhysicsScene.h"
#include "GameStateManager.h"

class App;
class Circle;
class Plane;
class Player;
class Box;

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

	float timer = 0.f;
	float timerReset = 0.1f;

	//Application2D* m_app;
	Player* m_player;
	Circle* m_ball;

	const float m_extents = 100;
	const float m_aspectRatio = 16.f / 9.f;

//====================
public:
	Circle* CreateCircle(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec4 a_colour, glm::vec2 a_force);
	Plane* CreatePlane(glm::vec2 a_normal, float a_distToOrigin, glm::vec4 a_colour);
	Box* CreateBox(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour, glm::vec2 a_force);
	Player* CreatePlayer(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec4 a_colour);
	Player* CreatePlayer(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour);
	void MouseInputTest(aie::Input* a_input);
	void ObjectTest();

	bool m_keyPressed = false;

	float sofi_DT;
};