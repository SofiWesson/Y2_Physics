#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height);
	Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height, glm::vec4 a_colour);
	Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, glm::vec2 a_dimensions);
	Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, glm::vec2 a_dimensions, glm::vec4 a_colour);
	~Box();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void MakeGizmo();

	bool CheckCorners(const Box& a_box, glm::vec2& a_contact, int& a_numContact, float& a_pen, glm::vec2& a_edgeNormal);

	float GetWidth() { return (m_extents.x * 2); }
	float GetHeight() { return (m_extents.y * 2); }
	glm::vec2 GetExtents() { return m_extents; }
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }

protected:
	glm::vec2 m_extents; // The half edge lengths
	glm::vec4 m_colour;

	// We will store the local x and y axes of the box based on its angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;

};