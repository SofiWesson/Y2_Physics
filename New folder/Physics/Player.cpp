#include "Player.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <Input.h>

Player::Player(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour) :
	RigidBody(CIRCLE, a_position, a_velocity, 0, a_mass) // no col, no inherit from circle
{
	m_radius = a_radius;
	m_colour = a_colour;
	m_isKinematic = false;
	m_moment = .5f * m_mass * a_radius * a_radius;
}

Player::Player(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height, glm::vec4 a_colour) :
	RigidBody(BOX, a_position, a_velocity, 0, a_mass) // no col, no inherit from box
{
	m_extents = glm::vec2(a_width / 2, a_height / 2);
	m_colour = a_colour;
	m_moment = 1.f / 12.f * a_mass * a_width * a_height;
}

Player::~Player()
{

}

void Player::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	RigidBody::FixedUpdate(a_gravity, a_timeStep);

	aie::Input* input = aie::Input::getInstance();

	if (this->GetShapeID() == BOX)
	{
		/* Fisrt store the local axes */
		float cs = cosf(m_rotation);
		float sn = sinf(m_rotation);
		m_localX = glm::normalize(glm::vec2(cs, sn));
		m_localY = glm::normalize(glm::vec2(-sn, cs));
	}

	PlayerController(this, input);
}

void Player::MakeGizmo()
{
	if (this->GetShapeID() == BOX)
	{
		/* Drawing using local axes */
		glm::vec2 p1 = m_positon - m_localX * m_extents.x - m_localY * m_extents.y;
		glm::vec2 p2 = m_positon + m_localX * m_extents.x - m_localY * m_extents.y;
		glm::vec2 p3 = m_positon - m_localX * m_extents.x + m_localY * m_extents.y;
		glm::vec2 p4 = m_positon + m_localX * m_extents.x + m_localY * m_extents.y;

		aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
		aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
	}

	if (this->GetShapeID() == CIRCLE)
		aie::Gizmos::add2DCircle(m_positon, m_radius, 12, m_colour);
}

void Player::PlayerController(Player* a_player, aie::Input* a_input)
{
	if (a_input->isKeyDown(aie::INPUT_KEY_W))
		a_player->ApplyForce(glm::vec2(0, 5.f), a_player->GetPosition());

	if (a_input->isKeyDown(aie::INPUT_KEY_A))
		a_player->ApplyForce(glm::vec2(-5.f, 0), a_player->GetPosition());

	if (a_input->isKeyDown(aie::INPUT_KEY_S))
		a_player->ApplyForce(glm::vec2(0, -5.f), a_player->GetPosition());

	if (a_input->isKeyDown(aie::INPUT_KEY_D))
		a_player->ApplyForce(glm::vec2(5.f, 0), a_player->GetPosition());
}