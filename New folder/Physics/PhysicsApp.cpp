#include <iostream>
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
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
#include "GameOverState.h"

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

	GameState* gs = m_app->GetGSM()->GetGameState("GameOver");
	m_gameOverState = (GameOverState*)gs;

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.01f);

	CreateTable();
	RackBalls();
	LoadUI();

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) 
{
	// check if balls are moving
	for (auto ball : m_balls)
	{
		if (ball->GetVelocity().x > 0.15f || ball->GetVelocity().x < -0.15f &&
			ball->GetVelocity().y > 0.15f || ball->GetVelocity().y < -0.15f)
		{
			m_ballsStatic = false;
			break;
		}
		if (ball == m_balls.back())
		{
			m_ballsStatic = true;
			m_inPlay = false;
		}
	}

	// change players turn if no balls sunk
	if (!m_inPlay && m_inPlayLastFrame && !m_hasBallBeenSunk)
		m_isPlayer1Turn = !m_isPlayer1Turn;

	// get input instance
	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed(aie::INPUT_KEY_1))
	{
		for each (Ball * ball in m_balls)
		{
			if (ball->GetBallType() == EIGHTBALL)
			{
				ball->SetPosition(glm::vec2(0, -20));
			}
			if (ball->GetBallType() == CUEBALL)
			{
				ball->SetPosition(glm::vec2(0, 0));
			}
		}
	}

	if (input->wasKeyPressed(aie::INPUT_KEY_2))
	{
		for each (Ball * ball in m_balls)
		{
			if (ball->GetBallType() == STRIPES)
			{
				ball->SetPosition(glm::vec2(0, -30));

			}
		}
	}

	// go back to menu
	if (input->wasKeyPressed(aie::INPUT_KEY_BACKSPACE))
	{
		m_app->GetGSM()->PopState();
		m_app->GetGSM()->PushState("Menu");
	}

	aie::Gizmos::clear();
	
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	HitCueBall(input);

	m_inPlayLastFrame = m_inPlay;
}

void PhysicsApp::draw()
{
	m_app->Get2DRenderer()->begin();

	// if the first ball has been sunk or not
	if (m_player1 == NUL && m_player2 == NUL)
		m_app->Get2DRenderer()->drawText(
			m_app->GetFont(),
			m_noBallsSunkText,
			(m_app->getWindowWidth() / 2) - (m_noBallsTextWidth / 2),
			m_app->getWindowHeight() - m_textHeight - 5);

	// TODO make black and white balls stop the other functions from changing players
	
	if (m_inPlay)
	{
		m_tableActivityText = m_waitText;
		m_tableActivityTextWidth = m_waitTextWidth / 2;
	}
	else
	{
		m_tableActivityText = m_goText;
		m_tableActivityTextWidth = m_goTextWidth / 2;
	}

	float tableActivityTextPos = m_firstBallHasBeenSunk ? m_textHeight + 5 : (m_textHeight * 2) + 20;

	// Tells the players weather the table is in play or not
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_tableActivityText,
		(m_app->getWindowWidth() / 2) - m_tableActivityTextWidth,
		m_app->getWindowHeight() - tableActivityTextPos);

	// displays which players turn it is with a white ball next to them
	float playerTurnRadius = 15.f;
	float playerTurnPosX = m_isPlayer1Turn ? m_player1TextWidth + 20 + playerTurnRadius : m_app->getWindowWidth() - m_player2TextWidth - 20 - playerTurnRadius;
	float playerTurnPosY = m_app->getWindowHeight() - (m_textHeight / 2) - 10;
	
	// player turn indicator
	m_app->Get2DRenderer()->drawCircle(
		playerTurnPosX,
		playerTurnPosY,
		playerTurnRadius);

	// Player 1
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_player1Text,
		10,
		m_app->getWindowHeight() - m_textHeight - 5);

	// Player 1 colour
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_colourText,
		10,
		m_app->getWindowHeight() - (m_textHeight * 2) - 20);

	// colour
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_player1ColourText,
		m_colourTextWidth + 30,
		m_app->getWindowHeight() - (m_textHeight * 2) - 20);

	// Player 2
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_player2Text,
		m_app->getWindowWidth() - m_player2TextWidth - 10,
		m_app->getWindowHeight() - m_textHeight - 5);

	// Player 2 colour
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_colourText,
		m_app->getWindowWidth() - m_colourTextWidth - 10,
		m_app->getWindowHeight() - (m_textHeight * 2) - 20);

	// colour
	m_app->Get2DRenderer()->drawText(
		m_app->GetFont(),
		m_player2ColourText,
		m_app->getWindowWidth() - m_colourTextWidth - m_player2ColourTextWidth - 30,
		m_app->getWindowHeight() - (m_textHeight * 2) - 20);

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
				float ballRadius = 2.f;

				if (ball->GetBallType() == SOLID) // SOLID is yellow
				{
					if (!m_firstBallHasBeenSunk)
					{
						m_firstBallHasBeenSunk = true;

						// sets what colour each player is
						m_player1 = m_isPlayer1Turn ? SOLID : STRIPES;
						m_player2 = m_isPlayer1Turn ? STRIPES : SOLID;

						m_player1ColourText = m_player1 == SOLID ? m_yellowText : m_redText;
						m_player2ColourText = m_player2 == SOLID ? m_yellowText : m_redText;

						m_player1ColourTextWidth = m_app->GetFont()->getStringWidth(m_player1ColourText);
						m_player2ColourTextWidth = m_app->GetFont()->getStringWidth(m_player2ColourText);
					}
					if (m_isPlayer1Turn)
					{
						// switch players turn
						m_isPlayer1Turn = m_player1 == SOLID ? true : false;

						ball->SetVelocity(glm::vec2(0, 0));

						float y = 40 - (ballRadius * 2.5f * m_p1Sunk.size());
						ball->SetPosition(glm::vec2(-70, y));

						// remove from active balls to check
						m_balls.remove(ball);
						m_p1Sunk.push_back(ball);
					}
					else if (!m_isPlayer1Turn)
					{
						m_isPlayer1Turn = m_player2 == STRIPES ? true : false;

						ball->SetVelocity(glm::vec2(0, 0));

						float y = 40 - (ballRadius * 2.5f * m_p2Sunk.size());
						ball->SetPosition(glm::vec2(70, y));

						// remove from active balls to check
						m_balls.remove(ball);
						m_p2Sunk.push_back(ball);
					}
				}
				else if (ball->GetBallType() == STRIPES) // same as SOLID above // STRIPES is red
				{
					if (!m_firstBallHasBeenSunk)
					{
						m_firstBallHasBeenSunk = true;

						m_player1 = m_isPlayer1Turn ? STRIPES : SOLID;
						m_player2 = m_isPlayer1Turn ? SOLID : STRIPES;

						m_player1ColourText = m_player1 == SOLID ? m_yellowText : m_redText;
						m_player2ColourText = m_player2 == SOLID ? m_yellowText : m_redText;

						m_player1ColourTextWidth = m_app->GetFont()->getStringWidth(m_player1ColourText);
						m_player2ColourTextWidth = m_app->GetFont()->getStringWidth(m_player2ColourText);
					}
					if (m_isPlayer1Turn)
					{
						m_isPlayer1Turn = m_player1 == STRIPES ? true : false;

						ball->SetVelocity(glm::vec2(0, 0));

						float y = 40 - (ballRadius * 2.5f * m_p1Sunk.size());
						ball->SetPosition(glm::vec2(-70, y));

						m_balls.remove(ball);
						m_p1Sunk.push_back(ball);
					}
					else if (!m_isPlayer1Turn)
					{
						m_isPlayer1Turn = m_player2 == SOLID ? true : false;

						ball->SetVelocity(glm::vec2(0, 0));

						float y = 40 - (ballRadius * 2.5f * m_p2Sunk.size());
						ball->SetPosition(glm::vec2(70, y));

						m_balls.remove(ball);
						m_p2Sunk.push_back(ball);
					}
				}
				else if (ball->GetBallType() == CUEBALL)
				{
					m_isPlayer1Turn = !m_isPlayer1Turn;
					ball->SetVelocity(glm::vec2(0, 0));
					ball->SetPosition(glm::vec2(-25, 0));
				}
				else if (ball->GetBallType() == EIGHTBALL)
				{
					bool containsStripes = false;
					bool containsSolids = false;

					for each (Ball* ball in m_balls)
					{
						if (ball->GetBallType() == STRIPES)
							containsStripes = true;
						else if (ball->GetBallType() == SOLID)
							containsSolids = true;
					}

					if (containsStripes)
					{
						if (!m_firstBallHasBeenSunk)
						{
							if (m_isPlayer1Turn)
								m_gameOverState->SetPlayWon("Player 2");
							else
								m_gameOverState->SetPlayWon("Player 1");

							m_app->GetGSM()->PushState("GameOver");
						}
						else if (m_isPlayer1Turn)
						{
							m_gameOverState->SetPlayWon("Player 2");
							m_app->GetGSM()->PushState("GameOver");
						}
						else
						{
							m_gameOverState->SetPlayWon("Player 1");
							m_app->GetGSM()->PushState("GameOver");
						}
					}
					else if (!containsStripes)
					{
						if (m_isPlayer1Turn && m_player1 == STRIPES)
						{
							m_gameOverState->SetPlayWon("Player 1");
							m_app->GetGSM()->PushState("GameOver");
						}
						else if (!m_isPlayer1Turn && m_player2 == STRIPES) // != STRIPES
						{
							m_gameOverState->SetPlayWon("Player 2");
							m_app->GetGSM()->PushState("GameOver");
						}
					}
					else if (containsSolids)
					{
						if (m_isPlayer1Turn)
						{
							m_gameOverState->SetPlayWon("Player 2");
							m_app->GetGSM()->PushState("GameOver");
						}
						else
						{
							m_gameOverState->SetPlayWon("Player 1");
							m_app->GetGSM()->PushState("GameOver");
						}
					}
					else
					{
						if (m_isPlayer1Turn && m_player1 == SOLID)
						{
							m_gameOverState->SetPlayWon("Player 1");
							m_app->GetGSM()->PushState("GameOver");
						}
						else if (!m_isPlayer1Turn && m_player2 == SOLID)
						{
							m_gameOverState->SetPlayWon("Player 2");
							m_app->GetGSM()->PushState("GameOver");
						}
					}

					ball->SetVelocity(glm::vec2(0, 0));
					ball->SetPosition(glm::vec2(0, 0));
				}

				m_hasBallBeenSunk = true;
			}
		};

		hole->triggerExit = [=](PhysicsObject* a_other)
		{

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
			m_inPlay = true;
			m_cueForceVectorStart = glm::vec2(0, 0);
			m_cue->ApplyForce(m_cueForce, glm::vec2(0, 0));
			m_hasBallBeenSunk = false;
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

void PhysicsApp::MouseInputTest(aie::Input* a_input) // not used
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

void PhysicsApp::ObjectTest() // not used
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

void PhysicsApp::LoadUI()
{
	m_textHeight = m_app->GetFont()->getStringHeight(m_player1Text);

	m_player1TextWidth = m_app->GetFont()->getStringWidth(m_player1Text);
	m_player2TextWidth = m_app->GetFont()->getStringWidth(m_player2Text);
	m_colourTextWidth = m_app->GetFont()->getStringWidth(m_colourText);
	m_noBallsTextWidth = m_app->GetFont()->getStringWidth(m_noBallsSunkText);
	m_yellowTextWidth = m_app->GetFont()->getStringWidth(m_yellowText);
	m_redTextWidth = m_app->GetFont()->getStringWidth(m_redText);
	m_waitTextWidth = m_app->GetFont()->getStringWidth(m_waitText);
	m_goTextWidth = m_app->GetFont()->getStringWidth(m_goText);
}