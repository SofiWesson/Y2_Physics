#include "Plane.h"
#include "RigidBody.h"
#include "PhysicsScene.h"

#include <Gizmos.h>
#include <glm/ext.hpp>
#include <iostream>

Plane::Plane(glm::vec2 a_normal, float a_distToOrigin, glm::vec4 a_colour) : PhysicsObject(PLANE)
{
	m_normal = a_normal;
	m_distToOrigin = a_distToOrigin;
	m_colour = a_colour;
	m_isKinematic = true;
	m_elasticity = 1;
	m_isTrigger = false;
}

Plane::Plane() : PhysicsObject(PLANE)
{
	m_normal = glm::vec2(0, 1);
	m_distToOrigin = 0.f;
	m_colour = glm::vec4(1, 1, 1, 1);
	m_isKinematic = true;
	m_elasticity = 1;
	m_isTrigger = false;
}

Plane::~Plane()
{

}

void Plane::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	
}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distToOrigin;
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

	aie::Gizmos::add2DLine(start, end, m_colour);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.f, m_colour, m_colour, colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.f, start - m_normal * 10.f, m_colour, colourFade, colourFade);
}

void Plane::ResolvePlaneCollision(RigidBody* a_rigidbody, glm::vec2 a_contact)
{
	/* The position at which we'll apply the force relative to the object's Center of Mass */
	glm::vec2 localContact = a_contact - a_rigidbody->GetPosition();

	// The plane will not move, so the relative velocity is just the Rigidbody's velocity at the contact point
	glm::vec2 vRel = a_rigidbody->GetVelocity() + a_rigidbody->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);

	float velocityIntoPlane = glm::dot(vRel, m_normal);
	
	// Perfect elasticity collision
	float e = (m_elasticity + a_rigidbody->GetElasticity()) / 2;

	/* Find the perpendicular distance so we can apply force at the relative center of mass, Torque = F*r */
	
	float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

	if (!m_isTrigger && !a_rigidbody->IsTrigger())
	{
		/* determine the 'effective mass' - this is the combination of the moment of inertia and mass,
	   this will tell us how much the contact point velocity will change with the forces applied */

		float eMass = 1.f / (1.f / a_rigidbody->GetMass() + (r * r) / a_rigidbody->GetMoment());

		float j = -(1 + e) * velocityIntoPlane * eMass;

		glm::vec2 force = m_normal * j;

		a_rigidbody->ApplyForce(force, a_contact - a_rigidbody->GetPosition());

		float pen = glm::dot(a_contact, m_normal) - m_distToOrigin;

		PhysicsScene::ApplyContactForces(a_rigidbody, nullptr, m_normal, pen);
	}
}
