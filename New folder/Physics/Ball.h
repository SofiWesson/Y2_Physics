#pragma once
#include "Circle.h"

class Ball : public Circle
{
public:
	Ball();
	Ball(BallType a_ballType, glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius);
	~Ball();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void MakeGizmo();

protected:
	BallType m_ballType;
};