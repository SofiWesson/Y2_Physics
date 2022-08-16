#pragma once
#include "RigidBody.h"

class Circle : public RigidBody
{
public:
	Circle();
	Circle(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour);
	~Circle();
	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void MakeGizmo();

	float GetRadius() { return m_radius; }
	glm::vec4 GetColour() { return m_colour; }
	glm::vec2 GetVelocityLastFrame() { return m_velocityLastFrame; }

	void SetRadius(float a_radius) { m_radius = a_radius; }
	void SetVelocityLastFrame(glm::vec2 a_currentVelocity) { m_velocityLastFrame = a_currentVelocity; }

protected:
	float m_radius;
	glm::vec4 m_colour;
	glm::vec2 m_velocityLastFrame = glm::vec2(0, 0);

	float m_dt;

};