#pragma once
#include "Circle.h"
#include <glm/ext.hpp>

class Ball : public Circle
{
public:
	Ball();
	Ball(BallType a_ballType, glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius);
	~Ball();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void MakeGizmo();

	BallType GetBallType() { return m_ballType; }

protected:
	BallType m_ballType;

private:
	glm::vec4 m_yellow = glm::vec4(1, 0.8f, 0, 1);
	glm::vec4 m_red = glm::vec4(1, 0, 0, 1);
	glm::vec4 m_black = glm::vec4(0, 0, 0, 1);
	glm::vec4 m_white = glm::vec4(1, 1, 1, 1);
	glm::vec4 m_magenta = glm::vec4(1, 0, 0.65f, 1);
};