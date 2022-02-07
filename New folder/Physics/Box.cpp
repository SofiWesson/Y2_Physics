#include "Box.h"
#include <Gizmos.h>

Box::Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height) :
	RigidBody(BOX, a_position, a_velocity, a_rotation, a_mass)
{
	m_extents = glm::vec2(a_width / 2, a_height / 2);
	m_colour = glm::vec4(0, 0, 1, 1);
	m_moment = 1.f / 12.f * a_mass * a_width * a_height;
}

Box::Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height, glm::vec4 a_colour) :
	RigidBody(BOX, a_position, a_velocity, a_rotation, a_mass)
{
	m_extents = glm::vec2(a_width / 2, a_height / 2);
	m_colour = a_colour;
	m_moment = 1.f / 12.f * a_mass * a_width * a_height;
}

Box::Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, glm::vec2 a_dimensions) :
	RigidBody(BOX, a_position, a_velocity, a_rotation, a_mass)
{
	m_extents = glm::vec2(a_dimensions.x / 2, a_dimensions.y / 2);
	m_colour = glm::vec4(0, 0, 1, 1);
	m_moment = 1.f / 12.f * a_mass * a_dimensions.x * a_dimensions.y;
}

Box::Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, glm::vec2 a_dimensions, glm::vec4 a_colour) :
	RigidBody(BOX, a_position, a_velocity, a_rotation, a_mass)
{
	m_extents = glm::vec2(a_dimensions.x / 2, a_dimensions.y / 2);
	m_colour = a_colour;
	m_moment = 1.f / 12.f * a_mass * a_dimensions.x * a_dimensions.y;
}

Box::~Box()
{

}

void Box::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	RigidBody::FixedUpdate(a_gravity, a_timeStep);

	/* Fisrt store the local axes */
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::MakeGizmo()
{
	/* Drawing using local axes */
	glm::vec2 p1 = m_positon - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_positon + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_positon - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_positon + m_localX * m_extents.x + m_localY * m_extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}