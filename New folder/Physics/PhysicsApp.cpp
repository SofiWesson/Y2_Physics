#include <iostream>
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <Input.h>
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Windows.h>

#include "App.h"
#include "PhysicsApp.h"
#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"
#include "Player.h"
#include "Box.h"
#include "Spring.h"
#include "Softbody.h"
#include "Ball.h"

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
	
	// Circle* ball1 = CreateCircle(glm::vec2(0, 0), glm::vec2(0, 0), 4.f, 4.f, glm::vec4(1, 0, 0.54f, 1), glm::vec2(0, 0), true, false, true);
	// Circle* ball2 = CreateCircle(glm::vec2(0, 0),  glm::vec2(0, 0), 4.f, 4.f, glm::vec4(0, 1, 0, 1), glm::vec2(0, 0), false, false, true);

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

	HitCueBall(input);
	// MouseInputTest(input);
}

void PhysicsApp::draw()
{
	
	m_app->Get2DRenderer()->begin();

	const char player1Text[] = "Player 1";
	const char player2Text[] = "Player 2";
	const char colourText[] = "Colour";
	const char noBallsSunk[] = "No Balls Sunk Yet";
	const char yellow[] = "Yellow";
	const char red[] = "Red";

	float player1Width = m_app->GetFont()->getStringWidth(player1Text);
	float player1Height = m_app->GetFont()->getStringHeight(player1Text);

	float player2Width = m_app->GetFont()->getStringWidth(player2Text);
	float player2Height = m_app->GetFont()->getStringHeight(player2Text);

	float colourTextWidth = m_app->GetFont()->getStringWidth(colourText);
	float colourTextHeight = m_app->GetFont()->getStringHeight(colourText);

	float noBallsTextWidth = m_app->GetFont()->getStringWidth(noBallsSunk);
	float noBallsTextHeight = m_app->GetFont()->getStringHeight(noBallsSunk);

	float yellowTextWidth = m_app->GetFont()->getStringWidth(yellow);
	float yellowTextHeight = m_app->GetFont()->getStringHeight(yellow);

	float redTextWidth = m_app->GetFont()->getStringWidth(red);
	float redTextHeight = m_app->GetFont()->getStringHeight(red);

	float player1ColourTextWidth = m_app->GetFont()->getStringWidth(m_player1Colour);
	float player1ColourTextHeight = m_app->GetFont()->getStringHeight(m_player1Colour);

	float player2ColourTextWidth = m_app->GetFont()->getStringWidth(m_player2Colour);
	float player2ColourTextHeight = m_app->GetFont()->getStringHeight(m_player2Colour);

	if (m_player1 == NUL && m_player2 == NUL)
		m_app->Get2DRenderer()->drawText(
			m_app->GetFont(),
			noBallsSunk,
			(m_app->getWindowWidth() / 2) - (noBallsTextWidth / 2),
			m_app->getWindowHeight() - noBallsTextHeight - 5);

	if (m_player1 == SOLID)
	{
		m_player1Colour = yellow;
		m_player2Colour = red;
	}
	else if (m_player1 == STRIPES)
	{
		m_player1Colour = red;
		m_player2Colour = yellow;
	}

	float playerTurnRadius = 15.f;
	float playerTurnPosX = isPlayer1Turn ? player1Width + 20 + playerTurnRadius : m_app->getWindowWidth() - player2Width - 20 - playerTurnRadius;
	float playerTurnPosY = m_app->getWindowHeight() - (player1Height / 2) - 10;
	

	m_app->Get2DRenderer()->drawCircle(
		playerTurnPosX,
		playerTurnPosY,
		playerTurnRadius);

	// Player 1
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		player1Text,
		10,
		m_app->getWindowHeight() - player1Height - 5);

	// Player 1 colour
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		colourText,
		10,
		m_app->getWindowHeight() - colourTextHeight - player1Height - 20);

	// colour
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_player1Colour,
		colourTextWidth + 30,
		m_app->getWindowHeight() - player1ColourTextHeight - player1Height - 20);

	// Player 2
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		player2Text,
		m_app->getWindowWidth() - player2Width - 10,
		m_app->getWindowHeight() - player2Height - 5);

	// Player 2 colour
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		colourText,
		m_app->getWindowWidth() - colourTextWidth - 10,
		m_app->getWindowHeight() - colourTextHeight - player2Height - 20);

	// colour
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_player2Colour,
		m_app->getWindowWidth() - colourTextWidth - player2ColourTextWidth - 30,
		m_app->getWindowHeight() - player2ColourTextHeight - player2Height - 20);

	m_app->Get2DRenderer()->end();
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

	//if (a_isTrigger)
	//{
	//	circle->triggerEnter = [=](PhysicsObject* a_other)
	//	{
	//		Ball* ball = dynamic_cast<Ball*>(a_other);
	//		if (ball != nullptr)
	//		{
	//			if (ball->GetBallType() == SOLID || STRIPES)
	//			{
	//				m_balls.remove(ball);
	//				m_physicsScene->RemoveActor(a_other);
	//			}
	//			else if (ball->GetBallType() == CUEBALL)
	//				ball->SetPosition(glm::vec2(-25, 0));
	//			else if (ball->GetBallType() == EIGHTBALL)
	//				ball->SetPosition(glm::vec2(0, 0));
	//		}

	//		//std::cout << trigger->GetPosition().x << " " << trigger->GetPosition().y << " Entered: " << a_other << std::endl;
	//	};
	//}

	return circle;
}

Ball* PhysicsApp::CreateBall(BallType a_ballType, glm::vec2 a_pos, glm::vec2 a_vel, float a_mass, float a_radius, glm::vec2 a_force, bool a_isKinematic, bool a_isTrigger, bool a_canCollide)
{
	Ball* ball= new Ball(a_ballType, a_pos, a_vel, a_mass, a_radius);

	ball->SetKinematic(a_isKinematic);
	ball->SetTrigger(a_isTrigger);
	ball->SetCollider(a_canCollide);

	m_physicsScene->AddActor(ball);

	ball->ApplyForce(a_force, glm::vec2(0, 0));

	return ball;
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

	float socketDiameter = 5.5f;

	std::list<Circle*> sockets;
	std::list<Circle*> triggers;

	Box* tableBase = CreateBox(pos, vel, rot, mass, width + edgeSize, height + edgeSize, baseColour, force, true, false, false);

	// ===================================================================== EDGES ===================================================================== // edges need resize
	// table north edge // left, right
	Box* tableNorthLeftEdge = CreateBox(
		glm::vec2(pos.x - (width / 4) - 0.4f,
			pos.y + height / 2 + edgeSize / 2),
		vel,
		rot,
		mass,
		width / 2 - socketDiameter - edgeSize + 1,
		edgeSize,
		edgeColour,
		force,
		true, false, true);

	Box* tableNorthRightEdge = CreateBox(
		glm::vec2(pos.x + (width / 4) + 0.4f,
			pos.y + height / 2 + edgeSize / 2),
		vel,
		rot,
		mass,
		width / 2 - socketDiameter - edgeSize + 1,
		edgeSize,
		edgeColour,
		force,
		true, false, true);

	// table south edge // left, right
	Box* tableSouthLeftEdge = CreateBox(
		glm::vec2(pos.x - (width / 4) - 0.4f,
			pos.y - height / 2 - edgeSize / 2),
		vel,
		rot,
		mass,
		width / 2 - socketDiameter - edgeSize + 1,
		edgeSize,
		edgeColour,
		force,
		true, false, true);

	Box* tableSouthRightEdge = CreateBox(
		glm::vec2(pos.x + (width / 4) + 0.4f,
			pos.y - height / 2 - edgeSize / 2),
		vel,
		rot,
		mass,
		width / 2 - socketDiameter - edgeSize + 1,
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
		height - socketDiameter - edgeSize / 1.5f,
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
		height - socketDiameter - edgeSize / 1.5f,
		edgeColour,
		force,
		true, false, true);

	// ===================================================================== SOCKET ENTERANCES =====================================================================
	// north
	Circle* northLeftLeft = CreateCircle(
		glm::vec2(tableNorthLeftEdge->GetPosition().x - tableNorthLeftEdge->GetWidth() / 2,
			tableNorthLeftEdge->GetPosition().y),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	Circle* northLeftRight = CreateCircle(
		glm::vec2(tableNorthLeftEdge->GetPosition().x + tableNorthLeftEdge->GetWidth() / 2,
			tableNorthLeftEdge->GetPosition().y),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	Circle* northRightLeft = CreateCircle(
		glm::vec2(tableNorthRightEdge->GetPosition().x - tableNorthRightEdge->GetWidth() / 2,
			tableNorthRightEdge->GetPosition().y),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	Circle* northRightRight = CreateCircle(
		glm::vec2(tableNorthRightEdge->GetPosition().x + tableNorthRightEdge->GetWidth() / 2,
			tableNorthRightEdge->GetPosition().y),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	// south
	Circle* southLeftLeft = CreateCircle(
		glm::vec2(tableSouthLeftEdge->GetPosition().x - tableSouthLeftEdge->GetWidth() / 2,
			tableSouthLeftEdge->GetPosition().y),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	Circle* southLeftRight = CreateCircle(
		glm::vec2(tableSouthLeftEdge->GetPosition().x + tableSouthLeftEdge->GetWidth() / 2,
			tableSouthLeftEdge->GetPosition().y),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	Circle* southRightLeft = CreateCircle(
		glm::vec2(tableSouthRightEdge->GetPosition().x - tableSouthRightEdge->GetWidth() / 2,
			tableSouthRightEdge->GetPosition().y),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	Circle* southRightRight = CreateCircle(
		glm::vec2(tableSouthRightEdge->GetPosition().x + tableSouthRightEdge->GetWidth() / 2,
			tableSouthRightEdge->GetPosition().y),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	// west
	Circle* westUp = CreateCircle(
		glm::vec2(tableWestEdge->GetPosition().x,
			tableWestEdge->GetPosition().y + tableWestEdge->GetHeight() / 2),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	Circle* westDown = CreateCircle(
		glm::vec2(tableWestEdge->GetPosition().x,
			tableWestEdge->GetPosition().y - tableWestEdge->GetHeight() / 2),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	// east
	Circle* eastUp = CreateCircle(
		glm::vec2(tableEastEdge->GetPosition().x,
			tableEastEdge->GetPosition().y + tableEastEdge->GetHeight() / 2),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	Circle* eastDown = CreateCircle(
		glm::vec2(tableEastEdge->GetPosition().x,
			tableEastEdge->GetPosition().y - tableEastEdge->GetHeight() / 2),
		vel,
		mass,
		edgeSize / 2,
		edgeColour,
		force,
		true, false, true);

	// ===================================================================== SOCKETS =====================================================================
	Circle* socketNorth = CreateCircle(
		glm::vec2(pos.x,
			pos.y + height / 2 + socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, true, true);

	sockets.push_back(socketNorth);

	Circle* socketSouth = CreateCircle(
		glm::vec2(pos.x,
			pos.y - height / 2 - socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, true, true);

	sockets.push_back(socketSouth);

	Circle* socketNW = CreateCircle(
		glm::vec2(pos.x - width / 2 + edgeSize / 2 - socketDiameter / 2,
			pos.y + height / 2 - edgeSize / 2 + socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, true, true);

	sockets.push_back(socketNW);

	Circle* socketSW = CreateCircle(
		glm::vec2(pos.x - width / 2 + edgeSize / 2 - socketDiameter / 2,
			pos.y - height / 2 + edgeSize / 2 - socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, true, true);

	sockets.push_back(socketSW);

	Circle* socketNE = CreateCircle(
		glm::vec2(pos.x + width / 2 - edgeSize / 2 + socketDiameter / 2,
			pos.y + height / 2 - edgeSize / 2 + socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, true, true);

	sockets.push_back(socketNE);

	Circle* socketSE = CreateCircle(
		glm::vec2(pos.x + width / 2 - edgeSize / 2 + socketDiameter / 2,
			pos.y - height / 2 + edgeSize / 2 - socketDiameter / 2),
		vel,
		mass,
		socketDiameter / 2,
		glm::vec4(.2f, .2f, .2f, 1),
		force,
		true, false, false);

	sockets.push_back(socketSE);

	// smaller trigger for sinking balls
	for (auto socket : sockets)
	{
		Circle* hole = CreateCircle(
			socket->GetPosition(),
			vel,
			mass,
			socketDiameter / 4,
			glm::vec4(
				socket->GetColour().x / 2,
				socket->GetColour().y / 2,
				socket->GetColour().z / 2,
				1),
			force,
			true, true, true);

		hole->triggerEnter = [=](PhysicsObject* a_other)
		{
			Ball* ball = dynamic_cast<Ball*>(a_other);
			if (ball != nullptr)
			{
				if (ball->GetBallType() == SOLID)
				{
					if (!ballHasBeenSunk)
					{
						ballHasBeenSunk = true;

						m_player1 = isPlayer1Turn ? SOLID : STRIPES;
						m_player2 = isPlayer1Turn ? STRIPES : SOLID;
					}
					else if (isPlayer1Turn) isPlayer1Turn = m_player1 == SOLID ? true : false;
					else if (!isPlayer1Turn) isPlayer1Turn = m_player2 == SOLID ? true : false;
				}
				else if (ball->GetBallType() == STRIPES)
				{
					if (!ballHasBeenSunk)
					{
						ballHasBeenSunk = true;

						m_player1 = isPlayer1Turn ? STRIPES : SOLID;
						m_player2 = isPlayer1Turn ? SOLID : STRIPES;
					}
					else if (isPlayer1Turn) isPlayer1Turn = m_player1 == STRIPES ? true : false;
					else if (!isPlayer1Turn) isPlayer1Turn = m_player2 == STRIPES ? true : false;
				}
				else if (ball->GetBallType() == CUEBALL)
				{
					isPlayer1Turn = !isPlayer1Turn;
					ball->SetVelocity(glm::vec2(0, 0));
					ball->SetPosition(glm::vec2(-25, 0));
				}
				else if (ball->GetBallType() == EIGHTBALL)
				{
					isPlayer1Turn = !isPlayer1Turn;
					ball->SetVelocity(glm::vec2(0, 0));
					ball->SetPosition(glm::vec2(0, 0));
				}
			}
		};
	}
		
}

void PhysicsApp::RackBalls()
{
	// ============================================ POOL BALL SPAWN ============================================ // enum with 4 different ball types
	Ball* ball = nullptr;
	
	glm::vec2 vel = glm::vec2(0, 0);
	float mass = 4;

	float circleRadius = 2.f;
	
	float xStart = 40.f;
	float yStart = 10.f;
	
	float xOffset = 5.f;
	float yOffset = 5.f;

	BallType ballType;
	
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
					ballType = SOLID;
				else
					ballType = STRIPES;
			}
			else if (x == 1)
			{
				if (y == 1 || y == 3)
					ballType = SOLID;
				else
					ballType = STRIPES;
			}
			else if (x == 2)
			{
				if (y == 0)
					ballType = SOLID;
				else if (y == 1)
					ballType = EIGHTBALL;
				else
					ballType = STRIPES;
			}
			else if (x == 3)
			{
				if (y == 1)
					ballType = SOLID;
				else
					ballType = STRIPES;
			}
			else
				ballType = STRIPES;

			ball = CreateBall(ballType, pos, vel, mass, circleRadius, glm::vec2(0, 0), false, false, true);
			ball->SetLinearDrag(0.45f);
			m_balls.push_back(ball);
		}
	}

	// cue ball
	m_cue = CreateBall(CUEBALL, glm::vec2(-25, 0), vel, mass, circleRadius, glm::vec2(0, 0), false, false, true);
	m_cue->SetLinearDrag(0.45f);
	m_balls.push_back(m_cue);
}

void PhysicsApp::HitCueBall(aie::Input* a_input)
{
	int screenX, screenY;

	glm::vec2 forceVector;

	if (a_input->isMouseButtonDown(0))
	{
		for (auto ball : m_balls)
		{
			if (ball->GetVelocity().x > 0.15f || ball->GetVelocity().x < -0.15f &&
				ball->GetVelocity().y > 0.15f || ball->GetVelocity().y < -0.15f)
			{
				m_ballsStatic = false;
				break;
			}
			if (ball == m_balls.back())
				m_ballsStatic = true;
		}

		if (m_ballsStatic)
		{
			a_input->getMouseXY(&screenX, &screenY);
			glm::vec2 worldPos = ScreenToWorld(glm::vec2(screenX, screenY));

			if (m_cueForceVectorStart == glm::vec2(0, 0))
				m_cueForceVectorStart = worldPos;

			glm::vec2 cuePos = m_cue->GetPosition();

			forceVector = glm::vec2(m_cueForceVectorStart - worldPos);
			
			float forceVectorLength = glm::length(forceVector);
			forceVectorLength = glm::clamp(forceVectorLength, 0.f, 20.f);

			float angle = atan2f(forceVector.y, forceVector.x);
			glm::vec2 end = glm::vec2(std::cos(angle), std::sin(angle)) * forceVectorLength;

			aie::Gizmos::add2DLine(cuePos, cuePos + end, glm::vec4(1, 0.5f, 0, 1)); // on release set cue velocity to forcevector

			float clampForce = 40.f;

			forceVector.x = glm::clamp(forceVector.x, -clampForce, clampForce);
			forceVector.y = glm::clamp(forceVector.y, -clampForce, clampForce);

			m_cueForce = forceVector * 20.f;
		}
	}
	if (a_input->wasMouseButtonReleased(0))
	{
		if (m_ballsStatic)
		{
			m_cueForceVectorStart = glm::vec2(0, 0);
			m_cue->ApplyForce(m_cueForce, glm::vec2(0, 0));
		}
	}
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