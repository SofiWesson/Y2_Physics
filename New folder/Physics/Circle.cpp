#include <Gizmos.h>
#include <glm/ext.hpp>

#include "Circle.h"
#include "RigidBody.h"
#include "PhysicsScene.h"

Circle::Circle() : RigidBody(CIRCLE)
{

}

Circle::Circle(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour) :
	RigidBody(CIRCLE, a_position, a_velocity, 0, a_mass)
{
	m_radius = a_radius;
	m_colour = a_colour;
	m_isKinematic = false;
	m_moment = .5f * m_mass * a_radius * a_radius;
}

Circle::~Circle()
{

}

void Circle::update(float a_dt)
{

}

void Circle::MakeGizmo()
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

void Circle::ResolveCircleCollision(Circle* a_otherActor, glm::vec2 a_contact, glm::vec2* a_collisionNormal, float a_pen)
{
//	double pi = 355 / 113;
//	
//
//	//=========================== CIRCLE 2 =====================================
//	float lengthOfRadius2 = glm::length(a_otherActor->GetRadius());
//	
//	glm::vec2 changeInVel2 = a_otherActor->GetVelocity() - a_otherActor->GetVelocityLastFrame();
//	
//	glm::vec2 acceleration2 = glm::vec2(changeInVel2.x / m_dt, changeInVel2.y / m_dt);
//	glm::vec2 force2 = a_otherActor->GetMass() * acceleration2;
//	float magnitudeForceVector2 = glm::length(force2);
//	
//	float angle2 = a_otherActor->GetRadius() * 180 / pi; // from radians to degrees
//	
//	float torque2 = lengthOfRadius2 * magnitudeForceVector2 * sin(angle2);
//
//	//=========================== COLLISION =====================================
//	
//	//// register that a collision has occured and these obejcts have overlapped
//	m_objectsInsideThisFrame.push_back(a_otherActor);
//	a_otherActor->m_objectsInsideThisFrame.push_back(this);
//
//	///* We need to find the vector between their centers or use the provided
//	//   directional force, and make sure it is normalised */
//	glm::vec2 normal = glm::normalize(a_collisionNormal ? *a_collisionNormal : a_otherActor->GetPosition() - m_positon);
//
//	///* Get the perpendicular vector to the collision normal */
//	glm::vec2 perpendicualColNorm(normal.y, -normal.x);
//	float radiusThis = glm::dot(a_contact - m_positon, -perpendicualColNorm);
//	float radiusOther = glm::dot(a_contact - a_otherActor->GetPosition(), perpendicualColNorm);
//
//	///* The velocity of the contact points */
//	float cp_velocityThis = glm::dot(m_velocity, normal) - radiusThis * m_angularVelocity;
//	float cp_velocityOther = glm::dot(a_otherActor->GetVelocity(), normal) + radiusOther * a_otherActor->GetAngularVelocity();
//
//	//=========================== CIRCLE 1 =====================================
//	float lengthOfRadius1 = glm::length(radiusThis);
//
//	glm::vec2 changeInVel1 = GetVelocity() - GetVelocityLastFrame();
//
//	glm::vec2 acceleration1 = changeInVel1 / m_dt;
//	glm::vec2 force1 = GetMass() * acceleration1;
//	float magnitudeForceVector1 = glm::length(force1);
//
//	float angle1 =  
//
//	//float torque1 = lengthOfRadius1 * magnitudeForceVector1 * sin(angle1);
//
//	float angleThis = 
//
//	if (cp_velocityThis > cp_velocityOther) // They are moving closer
//	{
//		if (!m_isTrigger && !a_otherActor->IsTrigger())
//		{
//			/* this will calculate the effective mass at the contact point for each other
//		   ie. How much the contact point  will move due to the forces applied */
//	
//			float massThis = 1.f / (1.f / GetMass() + glm::pow(radiusThis, 2.f) / m_moment);
//			float massOther = 1.f / (1.f / a_otherActor->GetMass() + glm::pow(radiusOther, 2.f) / a_otherActor->GetMoment());
//	
//			float elasticity = (m_elasticity + a_otherActor->GetElasticity()) / 2.f;
//	
//			glm::vec2 impact = (1.f + elasticity) * massThis * massOther / (massThis + massOther) * (cp_velocityThis - cp_velocityOther) * normal;
//			
//			SetMoment(torque1);
//			a_otherActor->SetMoment(torque2);
//
//			ApplyForce(-impact, a_contact - m_positon);
//			a_otherActor->ApplyForce(impact, a_contact - a_otherActor->GetPosition());
//		}
//		else
//		{
//			TriggerEnter(a_otherActor);
//			a_otherActor->TriggerEnter(this);
//		}
//	}
//	if (a_pen > 0)
//	{
//		PhysicsScene::ApplyContactForces(this, a_otherActor, normal, a_pen);
//	}
}