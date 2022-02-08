#include "Box.h"
#include <Gizmos.h>

Box::Box() : RigidBody(BOX)
{

}

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

// Problem with player box is this
bool Box::CheckCorners(const Box& a_box, glm::vec2& a_contact, int& a_numContact, float& a_pen, glm::vec2& a_edgeNormal)
{
	float minX = 0, maxX = 0, minY = 0, maxY = 0;
	float boxW = a_box.m_extents.x * 2;
	float boxH = a_box.m_extents.y * 2;
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;

	// loop over all coerners of the other box
	for (float x = -a_box.m_extents.x; x < boxW; x += boxW)
	{
		for (float y = -a_box.m_extents.y; y < boxH; y += boxH)
		{
			// get the position in worldspace
			glm::vec2 p = a_box.m_positon + x * a_box.m_localX + y * a_box.m_localY;
			// get the position in our box's space
			glm::vec2 p0(glm::dot(p - m_positon, m_localX), glm::dot(p - m_positon, m_localY));

			// update the extents in each cardinal direction in our box's space (ie extents along the separeting axes)
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			// if this corner is inside the box, add it to the list of contact points
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
				p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}
	
	// if we lie entirely to one side of the box along one axis, we've found a separting axis, and we can exit
	if (maxX <= -m_extents.x || minX >= m_extents.x ||
		maxY <= -m_extents.y || minY >= m_extents.y)
		return false;

	if (numLocalContacts == 0)
		return false;

	bool res = false;
	a_contact += m_positon + (localContact.x * m_localX + localContact.y * m_localY) / (float)numLocalContacts;
	a_numContact++;

	float pen0 = m_extents.x - minX;

	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = m_localX;
		a_pen = pen0;
		res = true;
	}

	pen0 = maxX + m_extents.x;

	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = -m_localX;
		a_pen = pen0;
		res = true;
	}

	pen0 = m_extents.y - minY;

	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = m_localY;
		a_pen = pen0;
		res = true;
	}

	pen0 = maxY + m_extents.y;

	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = -m_localY;
		a_pen = pen0;
		res = true;
	}

	return res;
}