#include <iostream>
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <Input.h>
#include "Texture.h"
#include "Font.h"
#include "Input.h"

#include "App.h"
#include "PhysicsApp.h"
#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"
#include "Player.h"
#include "Box.h"
#include "Spring.h"
#include "Softbody.h"

#define _USE_MATH_DEFINES
#include <math.h>

PhysicsApp::PhysicsApp()
{
	
}

PhysicsApp::~PhysicsApp()
{

}

bool PhysicsApp::startup(App* a_app)
{
	m_app = a_app;

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

	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.01f);

	CreateTable();

	RackBalls();

	// SoftbodyTest();

	// Plane* plane = CreatePlane(glm::vec2(0, 1), -30, glm::vec4(0, 1, 0, 1));

	//m_ball = CreateCircle(glm::vec2(0, 0), glm::vec2(0, 0), 4, 4, glm::vec4(0.5, 0.5, 0.5, 0.5), glm::vec2(0, 0), false, false);

	// Box* box1 = CreateBox(glm::vec2(20, 10), glm::vec2(0, 0), 0, 4.f, 4.f, 12.f, glm::vec4(0, 0, 1, 1), glm::vec2(-20, 0), false, false);
	// Box* box2 = CreateBox(glm::vec2(0, 0), glm::vec2(0, 0), 0, 4.f, 4.f, 12.f, glm::vec4(1, 0, 1, 1), glm::vec2(20, 0), false, false);
	
	// Circle* ball1 = CreateCircle(glm::vec2(0, 0), glm::vec2(0, 0), 4.f, 4.f, glm::vec4(1, 0, 0.54f, 1), glm::vec2(-20.f, 0), false, false);
	// ball1->SetRotation(20.f);
	// Circle* ball2 = CreateCircle(glm::vec2(-20, 0),  glm::vec2(0, 0), 4.f, 4.f, glm::vec4(0, 1, 0, 1), glm::vec2(20, 0), false, false);

	// m_player = CreatePlayer(glm::vec2(30, 0), glm::vec2(0, 0), 4.f, 4.f, glm::vec4(.5f, .5f, .5f, 1.f)); // cirlce
	// m_player = CreatePlayer(glm::vec2(-10, 0), glm::vec2(0, 0), 0, 4, 4, 8, glm::vec4(0, 0, 1, 1)); // box

	// CreateSpring(10);

	

	// ObjectTest(); 

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) 
{
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	MouseInputTest(input);
}

void PhysicsApp::draw()
{
	
}

glm::vec2 PhysicsApp::ScreenToWorld(glm::vec2 a_screenPos)
{
	glm::vec2 worldPos = a_screenPos;

	// Move the center of the screen to (0, 0)
	worldPos.x -= m_app->getWindowWidth() / 2;
	worldPos.y -= m_app->getWindowHeight() / 2;

	worldPos.x *= 2.f * m_extents / m_app->getWindowWidth();
	worldPos.y *= 2.f * m_extents / (m_aspectRatio * m_app->getWindowHeight());

	return worldPos;
}

Circle* PhysicsApp::CreateCircle(glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec4 a_colour, glm::vec2 a_force, bool a_isKinematic, bool a_isTrigger, bool a_canCollide)
{
	Circle* circle = new Circle(a_pos, a_vel, a_mass, a_radius, a_colour);

	circle->SetKinematic(a_isKinematic);
	circle->SetTrigger(a_isTrigger);
	circle->SetCollider(a_canCollide);

	m_physicsScene->AddActor(circle);

	circle->ApplyForce(a_force, glm::vec2(0, 0));

	return circle;
}

Plane* PhysicsApp::CreatePlane(glm::vec2 a_normal, float a_distToOrigin, glm::vec4 a_colour)
{
	Plane* plane = new Plane(a_normal, a_distToOrigin, a_colour);

	m_physicsScene->AddActor(plane);

	return plane;
}

Box* PhysicsApp::CreateBox(glm::vec2 a_pos, glm::vec2 a_vel, float a_rot, float a_mass, float a_width, float a_height, glm::vec4 a_colour, glm::vec2 a_force, bool a_isKinematic, bool a_isTrigger, bool a_canCollide)
{
	Box* box = new Box(a_pos, a_vel, a_rot, a_mass, a_width, a_height, a_colour);

	box->SetKinematic(a_isKinematic);
	box->SetTrigger(a_isTrigger);
	box->SetCollider(a_canCollide);

	m_physicsScene->AddActor(box);

	box->ApplyForce(a_force, glm::vec2(0, 0));

	return box;
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

void PhysicsApp::CreateSpring(int a_amount)
{
	Circle* prev = nullptr;
	for (int i = 0; i < a_amount; i++)
	{
		// Spawn new circle below the last
		Circle* circle = new Circle(glm::vec2(i * 3, 20 - i * 5), glm::vec2(0, 0), 10.f, 2.f, glm::vec4(0, 0, 1, 1));
		
		if (i == 0)
			circle->SetKinematic(true);

		m_physicsScene->AddActor(circle);
		if (prev)
			m_physicsScene->AddActor(new Spring(circle, prev, 10, 500));

		prev = circle;
	}

	// Add a kinematic box
	// Box* box = new Box(glm::vec2(0, -20), glm::vec2(0, 0), .3f, 20, glm::vec2(8, 2), glm::vec4(0, 1, 1, 1));
	// box->SetKinematic(true);
	// m_physicsScene->AddActor(box);
}

void PhysicsApp::CreateTable()
{
	glm::vec2 pos = glm::vec2(0, 0);
	glm::vec2 vel = glm::vec2(0, 0);
	float rot = 0.f;
	float mass = 0.f;
	float width = 100.f;
	float height = 50.f;
	glm::vec4 baseColour = glm::vec4(.14f, .48f, .18f, 1);
	glm::vec2 force = glm::vec2(0, 0);

	float edgeSize = 5;
	glm::vec4 edgeColour = glm::vec4(.5f, .3f, .04f, 1);
	bool isEdgeTrigger = false;

	float socketDiameter = 4.5f;

	Box* tableBase = CreateBox(pos, vel, rot, mass, width, height, baseColour, force, true, false, false);

	// table north edge // left, right
	Box* tableNorthLeftEdge = CreateBox(
		glm::vec2(pos.x - (width / 4),
				  pos.y + height / 2 + edgeSize / 2),
		vel,
		rot,
		mass,
		width / 2 - socketDiameter,
		edgeSize,
		edgeColour,
		force,
		true, false, true);
	
	Box* tableNorthRightEdge = CreateBox(
		glm::vec2(pos.x + (width / 4),
				  pos.y + height / 2 + edgeSize / 2),
		vel,
		rot,
		mass,
		width / 2 - socketDiameter,
		edgeSize,
		edgeColour,
		force,
		true, false, true);
	
	// table south edge // left, right
	Box* tableSouthLeftEdge = CreateBox(
		glm::vec2(pos.x - (width / 4),
				  pos.y - height / 2 - edgeSize / 2),
		vel,
		rot,
		mass,
		width / 2 - socketDiameter,
		edgeSize,
		edgeColour,
		force,
		true, false, true);
	
	Box* tableSouthRightEdge = CreateBox(
		glm::vec2(pos.x + (width / 4),
				  pos.y - height / 2 - edgeSize / 2),
		vel,
		rot,
		mass,
		width / 2 - socketDiameter,
		edgeSize,
		edgeColour,
		force,
		true, false, true);

	// table left edge
	Box* tableWestEdge = CreateBox(
		glm::vec2(pos.x - width / 2 - edgeSize / 2,
				  pos.y),
		vel,
		rot,
		mass,
		edgeSize,
		height - socketDiameter,
		edgeColour,
		force,
		true, false, true);

	// table right edge
	Box* tableEastEdge = CreateBox(
		glm::vec2(pos.x + width / 2 + edgeSize / 2,
				  pos.y),
		vel,
		rot,
		mass,
		edgeSize,
		height - socketDiameter,
		edgeColour,
		force,
		true, false, true);

	// sockets
	Circle* socketNorth = CreateCircle(
		glm::vec2(pos.x,
				  pos.y + height / 2 + socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, false, true);
	
	Circle* socketSouth = CreateCircle(
		glm::vec2(pos.x,
				  pos.y - height / 2 - socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, false, true);
	
	Circle* socketNW = CreateCircle(
		glm::vec2(pos.x - width / 2 + edgeSize / 2 - socketDiameter / 2,
				  pos.y + height / 2 - edgeSize / 2 + socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, false, true);

	Circle* socketSW = CreateCircle(
		glm::vec2(pos.x - width / 2 + edgeSize / 2 - socketDiameter / 2,
				  pos.y - height / 2 + edgeSize / 2 - socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, false, true);

	Circle* socketNE = CreateCircle(
		glm::vec2(pos.x + width / 2 - edgeSize / 2 + socketDiameter / 2,
				  pos.y + height / 2 - edgeSize / 2 + socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, false, true);

	Circle* socketSE = CreateCircle(
		glm::vec2(pos.x + width / 2 - edgeSize / 2 + socketDiameter / 2,
			      pos.y - height / 2 + edgeSize / 2 - socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, false, true);
}

void PhysicsApp::RackBalls()
{
	// ============================================ POOL BALL SPAWN ============================================ // enum with 4 different ball types
	Circle* circle = nullptr;
	
	glm::vec2 vel = glm::vec2(0, 0);
	float mass = 4;

	float circleRadius = 2.f;
	
	float xStart = 40.f;
	float yStart = 10.f;
	
	float xOffset = 5.f;
	float yOffset = 5.f;

	glm::vec4 colour;
	glm::vec4 yellow = glm::vec4(1, 0.8f, 0, 1);
	glm::vec4 red = glm::vec4(1, 0, 0, 1);
	glm::vec4 black = glm::vec4(0, 0, 0, 1);
	glm::vec4 white = glm::vec4(1, 1, 1, 1);
	
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5 - x; y++)
		{
			float xPos = xStart - (xOffset * x);
			float yPos = 1 + yStart - (yOffset / 2 * x) - (2 / circleRadius + yOffset * y);
	
			glm::vec2 pos = glm::vec2(xPos, yPos);
			
			if (x == 0)
			{
				if (y == 0 || y == 1 || y == 3)
					colour = yellow;
				else
					colour = red;
			}
			else if (x == 1)
			{
				if (y == 1 || y == 3)
					colour = yellow;
				else
					colour = red;
			}
			else if (x == 2)
			{
				if (y == 0)
					colour = yellow;
				else if (y == 1)
					colour = black;
				else
					colour = red;
			}
			else if (x == 3)
			{
				if (y == 1)
					colour = yellow;
				else
					colour = red;
			}
			else
				colour = red;

			circle = CreateCircle(pos, vel, mass, circleRadius, colour, glm::vec2(0, 0), false, false, true);
		}
	}

	// cue ball
	circle = CreateCircle(glm::vec2(-25, 0), vel, mass, circleRadius, white, glm::vec2(0, 0), false, false, true);
}

void PhysicsApp::SoftbodyTest()
{
	std::vector<std::string> sb;
	sb.push_back("0000.|.000000.|.00....|.000000");
	sb.push_back("0000.|.000000.|.00....|.000000");
	sb.push_back("..00.|.00..00.|.00....|...00..");
	sb.push_back("..00.|.00..00.|.00....|...00..");
	sb.push_back("0000.|.00..00.|.00000.|...00..");
	sb.push_back("0000.|.00..00.|.00000.|...00..");
	sb.push_back("00...|.00..00.|.00....|...00..");
	sb.push_back("00...|.00..00.|.00....|...00..");
	sb.push_back("0000.|.000000.|.00000.|.000000");
	sb.push_back("0000.|.000000.|.00000.|.000000");
	Softbody::Build(m_physicsScene, glm::vec2(-80, 0), 5, 2000, 1, sb);
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
		Circle* spawn = CreateCircle(worldPos, glm::vec2(0, 0), 4, 4, glm::vec4(.1f, .1f, .4f, 1.f), glm::vec2(0, 0), false, false, true);
	}
}

void PhysicsApp::ObjectTest()
{
	Circle* ball1 = CreateCircle(glm::vec2(10, 0), glm::vec2(0, 0), 4, 4, glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec2(0, 0), false, false, true);
	Circle* ball2 = CreateCircle(glm::vec2(10, -20), glm::vec2(0, 0), 4, 4, glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec2(0, 0), false, false, true);
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