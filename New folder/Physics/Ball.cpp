#include "Circle.h"
#include "RigidBody.h"
#include "PhysicsScene.h"
#include "Ball.h"

#include <Gizmos.h>
#include <glm/ext.hpp>

Ball::Ball()
{

}

Ball::Ball(BallType a_ballType, glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour) : Circle()
{
	m_radius = a_radius;
	m_colour = a_colour;
	m_isKinematic = false;
	m_moment = .5f * m_mass * a_radius * a_radius;
	m_ballType = a_ballType;
}

Ball::~Ball()
{

}

void Ball::update(float a_dt)
{

}

void Ball::MakeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;

	aie::Gizmos::add2DCircle(m_positon, m_radius, 12, m_colour);

	float r = OpposingColour(m_colour.r);
	float g = OpposingColour(m_colour.g);
	float b = OpposingColour(m_colour.b);
	float a = m_colour.a;

	glm::vec4 newColour = glm::vec4(r, g, b, a);
	aie::Gizmos::add2DLine(m_positon, m_positon + end, newColour);
}
