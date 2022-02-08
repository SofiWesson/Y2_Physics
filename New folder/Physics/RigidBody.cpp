#include "RigidBody.h"
#include <glm/glm.hpp>
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <iostream>
#include <math.h>

#include <string>
#include <stdio.h>
#include <ctype.h>

#define MIN_LINEAR_THRESHOLD 0
#define MIN_ANGULAR_THRESHOLD 0

RigidBody::RigidBody(ShapeType a_shapeID) : PhysicsObject(a_shapeID)
{
}

RigidBody::RigidBody(
	ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass) :
	PhysicsObject(a_shapeID)
{
	m_positon = a_position;
	m_velocity = a_velocity;
	m_rotation = a_rotation;
	m_mass = a_mass;
	m_elasticity = 1;
	m_angularVelocity = 0;
}

RigidBody::~RigidBody()
{

}

void RigidBody::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	m_positon += GetVelocity() * a_timeStep;
	ApplyForce(a_gravity * GetMass() * a_timeStep, glm::vec2(0, 0));

	m_rotation += m_angularVelocity * a_timeStep;

	m_velocity -= m_velocity * m_linearDrag * a_timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * a_timeStep;

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity = glm::vec2(0, 0);

	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
		m_angularVelocity = 0;
}

void RigidBody::ResolveCollision(RigidBody* a_otherActor, glm::vec2 a_contact, glm::vec2* a_collisionNormal, float pen)
{
	/* We need to find the vector between their centers or use the provided
	   directional force, and make sure it is normalised */

	glm::vec2 normal = glm::normalize(a_collisionNormal ? *a_collisionNormal :
		a_otherActor->GetPosition() - m_positon);

	/* Get the perpendicular vector to the collision normal */
	glm::vec2 perpendicualColNorm(normal.y, -normal.x);
	float radiusThis = glm::dot(a_contact - m_positon, -perpendicualColNorm);
	float radiusOther = glm::dot(a_contact - a_otherActor->GetPosition(), perpendicualColNorm);

	/* The velocity of the contact points */
	float cp_velocityThis = glm::dot(m_velocity, normal) - radiusThis * m_angularVelocity;
	float cp_velocityOther = glm::dot(a_otherActor->GetVelocity(), normal) + radiusOther * a_otherActor->GetAngularVelocity();

	if (cp_velocityThis > cp_velocityOther) // They are moving closer
	{
		/* this will calculate the effective mass at the contact point for each other
		   ie. How much the contact point  will move due to the forces applied */

		float massThis = 1.f / (1.f / m_mass + glm::pow(radiusThis, 2.f) / m_moment);
		float massOther = 1.f / (1.f / a_otherActor->GetMass() + glm::pow(radiusOther, 2.f) / a_otherActor->GetMoment());

		float elasticity = (m_elasticity + a_otherActor->GetElasticity()) / 2.f;

		glm::vec2 impact = (1.f + elasticity) * massThis * massOther / (massThis + massOther) * (cp_velocityThis - cp_velocityOther) * normal;

		ApplyForce(-impact, a_contact - m_positon);
		a_otherActor->ApplyForce(impact, a_contact - a_otherActor->GetPosition());
	}
}

void RigidBody::ApplyForce(glm::vec2 a_force, glm::vec2 a_contact)
{
	m_velocity += a_force / GetMass();
	m_angularVelocity += (a_force.y * a_contact.x - a_force.x * a_contact.y) / GetMoment();
}

float RigidBody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}

float RigidBody::OpposingColour(float a_value)
{
	// percentile to rgb
	float rgbValue = glm::round(a_value * 255);

	// convert rgb to hex values
	float fHexValue = rgbValue / 16;
	int hex1Value = glm::round(fHexValue);
	int hex2Value = (fHexValue - hex1Value) * 16;

	// invert hex values
	hex1Value = 15 - hex1Value;
	hex2Value = 15 - hex2Value;

	// convert hex values back into rgb
	rgbValue = (hex1Value * 16) + hex2Value;

	// convert rgb back into percentile
	float percentile = rgbValue / 255;

	return percentile;
}