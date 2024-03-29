#include "RigidBody.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"

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
	m_isKinematic = false;
	m_isTrigger = false;
}

RigidBody::~RigidBody()
{

}

void RigidBody::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	if (m_isTrigger)
	{
		/* Check every object that is inside this object and called triggerEnter on if they haven't
		   registered inside of this frame, they had to of exited therefore we can remove them
		   from out list and call triggerExit */

		for (auto it = m_objectsInside.begin(); it != m_objectsInside.end(); it++)
		{
			if (std::find(m_objectsInsideThisFrame.begin(), m_objectsInsideThisFrame.end(), *it) == m_objectsInsideThisFrame.end())
			{
				if (triggerExit)
				{
					triggerExit(*it);
					it = m_objectsInside.erase(it);
					if (it == m_objectsInside.end())
						break;
				}
			}
		}
	}
	// clear the list now for the next frame
	m_objectsInsideThisFrame.clear();

	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0, 0);
		m_angularVelocity = 0;
		return;
	}

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

void RigidBody::ResolveCollision(RigidBody* a_otherActor, glm::vec2 a_contact, glm::vec2* a_collisionNormal, float a_pen)
{
	// register that a collision has occured and these obejcts have overlapped
	m_objectsInsideThisFrame.push_back(a_otherActor);
	a_otherActor->m_objectsInsideThisFrame.push_back(this);

	/* We need to find the vector between their centers or use the provided
	   directional force, and make sure it is normalised */

	glm::vec2 normal;

	if (a_collisionNormal == nullptr)
		if (a_otherActor->GetPosition() - m_positon == glm::vec2(0, 0))
			normal = glm::vec2(0.1f, 0);
		else
			normal = glm::normalize(a_otherActor->GetPosition() - m_positon);
	else
		normal = glm::normalize(*a_collisionNormal);

	/* Get the perpendicular vector to the collision normal */
	glm::vec2 perpendicualColNorm(normal.y, -normal.x);
	float radiusThis = glm::dot(a_contact - m_positon, -perpendicualColNorm);
	float radiusOther = glm::dot(a_contact - a_otherActor->GetPosition(), perpendicualColNorm);

	/* The velocity of the contact points */
	float cp_velocityThis = glm::dot(m_velocity, normal) - radiusThis * m_angularVelocity;
	float cp_velocityOther = glm::dot(a_otherActor->GetVelocity(), normal) + radiusOther * a_otherActor->GetAngularVelocity();

	if (cp_velocityThis >= cp_velocityOther) // They are moving closer
	{
		if (!IsTrigger() && !a_otherActor->IsTrigger())
		{
			/* this will calculate the effective mass at the contact point for each other
		    ie. How much the contact point will move due to the forces applied */

			float massThis = 0.f;
			float massOther = 0.f;
			float elasticity = 0.f;
			glm::vec2 impact = glm::vec2(0, 0);

			if (GetIsKinematic() && a_otherActor->GetIsKinematic())
				return;

			if (!GetIsKinematic() && !a_otherActor->GetIsKinematic())
			{
				massThis = 1.f / (1.f / GetMass() + glm::pow(radiusThis, 2.f) / m_moment);
				massOther = 1.f / (1.f / a_otherActor->GetMass() + glm::pow(radiusOther, 2.f) / a_otherActor->GetMoment());

				elasticity = (m_elasticity + a_otherActor->GetElasticity()) / 2.f;

				impact = (1.f + elasticity) * massThis * massOther / (massThis + massOther) * (cp_velocityThis - cp_velocityOther) * normal;

				ApplyForce(-impact, a_contact - m_positon);
				a_otherActor->ApplyForce(impact, a_contact - a_otherActor->GetPosition());
			}
			
			if (GetIsKinematic() && !a_otherActor->GetIsKinematic()) // other actor collide normally
			{
				glm::vec2 localContact = a_contact - a_otherActor->GetPosition();
				glm::vec2 vRel = a_otherActor->GetVelocity() + a_otherActor->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);

				float velIntoThis = glm::dot(vRel, normal);
				float e = (m_elasticity + a_otherActor->GetElasticity()) / 2;
				float r = glm::dot(localContact, glm::vec2(normal.y, -normal.x));

				float eMass = 1.f / (1.f / a_otherActor->GetMass() + (r * r) / a_otherActor->GetMoment());
				float j = -(1 + e) * velIntoThis * eMass;

				glm::vec2 force = normal * j;

				a_otherActor->ApplyForce(force, a_contact - a_otherActor->GetPosition());
			}

			if (a_otherActor->GetIsKinematic() && !GetIsKinematic()) // this actor collide normally
			{
				glm::vec2 localContact = a_contact - GetPosition();
				glm::vec2 vRel = GetVelocity() + GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);

				float velIntoOther = glm::dot(vRel, normal);
				float e = (a_otherActor->GetElasticity() + GetElasticity()) / 2;
				float r = glm::dot(localContact, glm::vec2(normal.y, -normal.x));

				float eMass = 1.f / (1.f / GetMass() + (r * r) / GetMoment());
				float j = -(1 + e) * velIntoOther * eMass;

				glm::vec2 force = normal * j;

				ApplyForce(force, a_contact - GetPosition());
			}
		}
		else
		{
			TriggerEnter(a_otherActor);
			a_otherActor->TriggerEnter(this);
		}

		if (a_pen > 0)
		{
			PhysicsScene::ApplyContactForces(this, a_otherActor, normal, a_pen);
		}
	}
}

void RigidBody::ApplyForce(glm::vec2 a_force, glm::vec2 a_contact)
{
	m_velocity += a_force / GetMass();
	m_angularVelocity += (a_force.y * a_contact.x - a_force.x * a_contact.y) / GetMoment();
}

glm::vec2 RigidBody::ToWorld(glm::vec2 a_localPos)
{
	return m_positon + m_localX * a_localPos.x + m_localY * a_localPos.y;
}

float RigidBody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}

void RigidBody::TriggerEnter(PhysicsObject* a_otherObject)
{
	if (m_isTrigger && std::find(m_objectsInside.begin(), m_objectsInside.end(), a_otherObject) == m_objectsInside.end())
	{
		m_objectsInside.push_back(a_otherObject);
		if (triggerEnter != nullptr)
			triggerEnter(a_otherObject);
	}
}

float RigidBody::OpposingColour(float a_value)
{
	// invert value
	return 1.0f - a_value;
}