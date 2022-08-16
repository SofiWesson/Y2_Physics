#include "Circle.h"
#include "RigidBody.h"
#include "PhysicsScene.h"
#include "Ball.h"

#include <Gizmos.h>
#include <glm/ext.hpp>

Ball::Ball()
{

}

Ball::Ball(BallType a_ballType, glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius) : Circle(a_position, a_velocity, a_mass, a_radius, glm::vec4(0, 0, 0, 0))
{
	m_isKinematic = false;
	m_moment = .5f * m_mass * a_radius * a_radius;
	m_ballType = a_ballType;
	
	if (a_ballType == SOLID)
		m_colour = m_yellow;
	else if (a_ballType == STRIPES)
		m_colour = m_red;
	else if (a_ballType == EIGHTBALL)
		m_colour = m_black;
	else if (a_ballType == CUEBALL)
		m_colour = m_white;
	else
		m_colour = m_magenta;
}

Ball::~Ball()
{

}

void Ball::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	RigidBody::FixedUpdate(a_gravity, a_timeStep);
}

void Ball::MakeGizmo(){
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;

	aie::Gizmos::add2DCircle(m_positon, m_radius, 12, m_colour);

	float r = OpposingColour(m_colour.r);
	float g = OpposingColour(m_colour.g);
	float b = OpposingColour(m_colour.b);
	float a = m_colour.a;

	glm::vec4 newColour = glm::vec4(r, g, b, a);
	aie::Gizmos::add2DLine(m_positon, m_positon + end, newColour);
}
