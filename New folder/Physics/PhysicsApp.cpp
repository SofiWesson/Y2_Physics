#include <iostream>
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <Input.h>
#include "Texture.h"
#include "Font.h"
#include "Input.h"

#include "Application.h"
#include "PhysicsApp.h"
#include "Circle.h"
#include "Plane.h"
#include "Player.h"
#include "Box.h"

#define _USE_MATH_DEFINES
#include <math.h>

PhysicsApp::PhysicsApp() : Application()
{
	
}

PhysicsApp::~PhysicsApp()
{

}

bool PhysicsApp::startup() // game manager
{
	// Increase the 2d line count to maximise the number of objects we can draw

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();

	/* The lower the value the more accurate the simulation will be. 
	   but it will increase the processing time required. If it is too high
	   it will cause the sim to stutter and reduce the stability */

	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));
	m_physicsScene->SetTimeStep(0.01f);

	Plane* plane = CreatePlane(glm::vec2(0, 1), -30, glm::vec4(0, 1, 0, 1));

	//m_ball = CreateCircle(glm::vec2(0, 0), glm::vec2(0, 0), 4, 4, glm::vec4(0.5, 0.5, 0.5, 0.5), glm::vec2(0, 0));

	//Box* box1 = CreateBox(glm::vec2(0, 20), glm::vec2(0, 0), 0, 4.f, 8.f, 4.f, glm::vec4(0, 0, 1, 1), glm::vec2(0, 0));
	//Box* box2 = CreateBox(glm::vec2(0, 0), glm::vec2(0, 0), 0, 4.f, 8.f, 4.f, glm::vec4(1, 0, 1, 1), glm::vec2(0, 0));
	
	Circle* ball1 = CreateCircle(glm::vec2(0, 10), glm::vec2(0, 0), 4.f, 4.f, glm::vec4(1, 0, 0.54f, 1), glm::vec2(0, 0));
	//Circle* ball2 = CreateCircle(glm::vec2(0, 30),  glm::vec2(0, 0), 4.f, 4.f, glm::vec4(0, 1, 0, 1), glm::vec2(0, 0));

	//m_player = CreatePlayer(glm::vec2(30, 0), glm::vec2(0, 0), 4.f, 4.f, glm::vec4(.5f, .5f, .5f, 1.f)); // cirlce
	m_player = CreatePlayer(glm::vec2(-10, 0), glm::vec2(0, 0), 0, 4, 4, 8, glm::vec4(0, 0, 1, 1)); // box

	ObjectTest();

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	MouseInputTest(input);
}

void PhysicsApp::draw()
{
	
}

glm::vec2 PhysicsApp::ScreenToWorld(glm::vec2 a_screenPos)
{
	glm::vec2 worldPos = a_screenPos;

	// Move the center of the screen to (0, 0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	worldPos.x *= 2.f * m_extents / getWindowWidth();
	worldPos.y *= 2.f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}

Circle* PhysicsApp::CreateCircle(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec4 a_colour, glm::vec2 a_force)
{
	Circle* circle = new Circle(a_pos, a_vel, a_mass, a_radius, a_colour);

	m_physicsScene->AddActor(circle);

	circle->ApplyForce(a_force, circle->GetPosition());

	return circle;
}

Plane* PhysicsApp::CreatePlane(glm::vec2 a_normal, float a_distToOrigin, glm::vec4 a_colour)
{
	// Plane* testPlane1 = new Plane(glm::vec2(1, 0), -40,  glm::vec4(0, 1, 0, 1)); // left
	// Plane* testPlane2 = new Plane(glm::vec2(-1, 0), -40, glm::vec4(0, 1, 0, 1)); // right
	// Plane* testPlane3 = new Plane(glm::vec2(0, 1), -40,  glm::vec4(0, 1, 0, 1)); // bottom
	// Plane* testPlane4 = new Plane(glm::vec2(1, 1), -32,  glm::vec4(0, 1, 0, 1)); // bottom left
	// Plane* testPlane5 = new Plane(glm::vec2(-1, 1), -32, glm::vec4(0, 1, 0, 1)); // bottom right

	// m_physicsScene->AddActor(testPlane1);
	// m_physicsScene->AddActor(testPlane2);
    // m_physicsScene->AddActor(testPlane3);
	// m_physicsScene->AddActor(testPlane4);
	// m_physicsScene->AddActor(testPlane5);

	Plane* plane = new Plane(a_normal, a_distToOrigin, a_colour);

	m_physicsScene->AddActor(plane);

	return plane;
}

Player* PhysicsApp::CreatePlayer(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec4 a_colour)
{
	Player* player = new Player(a_pos, a_vel, a_mass, a_radius, a_colour);

	m_physicsScene->AddActor(player);

	return player;
}

Player* PhysicsApp::CreatePlayer(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour)
{
	Player* player = new Player(a_pos, a_vel, a_rot, a_mass, a_width, a_height, a_colour);

	m_physicsScene->AddActor(player);

	return player;
}

Box* PhysicsApp::CreateBox(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour, glm::vec2 a_force)
{
	Box* box = new Box(a_pos, a_vel, a_rot, a_mass, a_width, a_height, a_colour);

	m_physicsScene->AddActor(box);

	box->ApplyForce(a_force, box->GetPosition());

	return box;
}

void PhysicsApp::MouseInputTest(aie::Input* a_input)
{
	int screenX, screenY;

	if (a_input->isMouseButtonDown(0))
	{
		a_input->getMouseXY(&screenX, &screenY);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(screenX, screenY));

		aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(.1f, .1f, .9f, 1.f));
	}
	if (a_input->wasMouseButtonReleased(0))
	{
		a_input->getMouseXY(&screenX, &screenY);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(screenX, screenY));
		Circle* spawn = CreateCircle(worldPos, glm::vec2(0, 0), 4, 4, glm::vec4(.1f, .1f, .4f, 1.f), glm::vec2(0, 0));
	}
}

void PhysicsApp::ObjectTest()
{
	Circle* ball1 = CreateCircle(glm::vec2(10, 0), glm::vec2(0, 0), 4, 4, glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec2(0, 0));
	Circle* ball2 = CreateCircle(glm::vec2(10, -20), glm::vec2(0, 0), 4, 4, glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec2(0, 0));
	ball2->SetKinematic(true);
	ball2->SetTrigger(true);

	Plane* plane1 = CreatePlane(glm::vec2(0, 1), -30, glm::vec4(1, 1, 1, 1));
	Plane* plane2 = CreatePlane(glm::vec2(0, -1), -30, glm::vec4(1, 1, 1, 1));
	Plane* plane3 = CreatePlane(glm::vec2(1, 0), -30, glm::vec4(1, 1, 1, 1));
	Plane* plane4 = CreatePlane(glm::vec2(-1, 0), -30, glm::vec4(1, 1, 1, 1));

	ball2->triggerEnter = [=](PhysicsObject* a_other)
	{
		std::cout << "Entered: " << a_other << std::endl;
	};
	ball2->triggerExit = [=](PhysicsObject* a_other)
	{
		std::cout << "Exited: " << a_other << std::endl;
	};
}