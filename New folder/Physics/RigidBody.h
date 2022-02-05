#pragma once
#include <glm/glm.hpp>
#include "PhysicsObject.h"
#include <iostream>
#include <functional>

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass);
	~RigidBody();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void Debug() { std::cout << "position: " << m_positon.x << ', ' << m_positon.y << std::endl; }
	
	void ResolveCollision(RigidBody* a_otherActor, glm::vec2 a_contact, glm::vec2* a_collisionNormal = nullptr);

	void ApplyForce(glm::vec2 a_force, glm::vec2 a_contact);

	glm::vec2 GetPosition() { return m_positon; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetRotation() { return m_rotation; }
	float GetMass() { return m_mass; }
	float GetAngularVelocity() { return m_angularVelocity; }
	float GetMoment() { return m_moment; }
	bool GetIsKinematic() { return m_isKinematic; }

	float GetKineticEnergy();

	void SetMass(float a_mass) { m_mass = a_mass; }

protected:
	glm::vec2 m_positon;
	glm::vec2 m_velocity;
	float m_rotation; // In 2D we only need a single float
	float m_mass;
	float m_angularVelocity;
	float m_moment;

};

