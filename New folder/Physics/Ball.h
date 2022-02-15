#pragma once
#include "Circle.h"

class Ball : public Circle
{
public:
	Ball();
	Ball(BallType a_ballType, glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour);
	~Ball();

	virtual void update(float a_dt);
	virtual void MakeGizmo();

protected:
	BallType m_ballType;

};