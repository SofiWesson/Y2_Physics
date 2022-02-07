#include <iostream>
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <Input.h>

#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Circle.h"
#include "Plane.h"
#include "Player.h"
#include "Box.h"

#define _USE_MATH_DEFINES
#include <math.h>

PhysicsApp::PhysicsApp()
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

	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.01f);

	Plane* plane = CreatePlane(glm::vec2(0, 1), -30, glm::vec4(0, 1, 0, 1));

	Box* box = CreateBox(glm::vec2(0, 0), glm::vec2(0, 0), 1, 4.f, 8.f, 4.f, glm::vec4(0, 0, 1, 1), glm::vec2(0, 0));
	
	Circle* ball1 = CreateCircle(glm::vec2(0, 20), glm::vec2(0, 0), 4.f, 4.f, glm::vec4(1, 0, 0.54f, 1), glm::vec2(0, -50));
	// Circle* ball2 = CreateCircle(glm::vec2(10, 0),  glm::vec2(0, 0), 4.f, 4.f, glm::vec4(0, 1, 0, 1), glm::vec2(0, 0));

	// m_player = CreatePlayer(glm::vec2(10, 0), glm::vec2(0, 0), 4.f, 4.f, glm::vec4(.5f, .5f, .5f, 1.f)); // cirlce
	// m_player = CreatePlayer(glm::vec2(-10, 0), glm::vec2(0, 0), 0, 4, 4, 8, glm::vec4(0, 0, 1, 1)); // box

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

	PlayerControl(m_player, input);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));

	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 10);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::PlayerControl(Player* a_player, aie::Input* a_input)
{
	if (a_input->isKeyDown(aie::INPUT_KEY_W))
		a_player->ApplyForce(glm::vec2(0, 5.f), a_player->GetPosition());

	if (a_input->isKeyDown(aie::INPUT_KEY_A))
		a_player->ApplyForce(glm::vec2(-5.f, 0), a_player->GetPosition());

	if (a_input->isKeyDown(aie::INPUT_KEY_S))
		a_player->ApplyForce(glm::vec2(0, -5.f), a_player->GetPosition());

	if (a_input->isKeyDown(aie::INPUT_KEY_D))
		a_player->ApplyForce(glm::vec2(5.f, 0), a_player->GetPosition());
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
	// 
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