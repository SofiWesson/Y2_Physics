#pragma once
#include "Circle.h"
#include "Box.h"
#include "Player.h"
#include "RigidBody.h"

#include <Input.h>

class Player : public RigidBody
{
public:
	Player(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour);
	Player(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height, glm::vec4 a_colour);
	~Player();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void MakeGizmo();

	void PlayerController(Player* a_player, aie::Input* a_input);

	bool CheckCorners(const Player& a_box, glm::vec2& a_contact, int& a_numContact, float& a_pen, glm::vec2& a_edgeNormal);

	float GetRadius() { return m_radius; }
	glm::vec4 GetColour() { return m_colour; }

	float GetWidth() { return (m_extents.x * 2); }
	float GetHeight() { return (m_extents.y * 2); }
	glm::vec2 GetExtents() { return m_extents; }
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }

protected:

	float m_radius;
	glm::vec4 m_colour;

	glm::vec2 m_extents; // The half edge lengths

	// We will store the local x and y axes of the box based on its angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;

};