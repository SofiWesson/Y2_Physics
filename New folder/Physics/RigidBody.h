#pragma once
#include <glm/glm.hpp>
#include "PhysicsObject.h"
#include <iostream>
#include <functional>
#include <list>

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType a_shapeID);
	RigidBody(ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass);
	~RigidBody();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void Debug() { std::cout << "position: " << m_positon.x << ', ' << m_positon.y << std::endl; }
	
	void ResolveCollision(RigidBody* a_otherActor, glm::vec2 a_contact, glm::vec2* a_collisionNormal = nullptr, float pen = 0);

	void ApplyForce(glm::vec2 a_force, glm::vec2 a_contact);

	glm::vec2 ToWorld(glm::vec2 a_localPos);

	// Getters
	glm::vec2 GetPosition() { return m_positon; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetRotation() { return m_rotation; }
	float GetMass() { return m_isKinematic ? INT_MAX : m_mass; }
	float GetAngularVelocity() { return m_angularVelocity; }
	float GetMoment() { return m_isKinematic ? INT_MAX : m_moment; }
	bool GetIsKinematic() { return m_isKinematic; }
	float GetKineticEnergy();
	float GetLinearDrag() { return m_linearDrag; }

	// Setters
	void SetMass(float a_mass) { m_mass = a_mass; }
	void SetKinematic(bool a_state) { m_isKinematic = a_state; }
	void SetPosition(glm::vec2 a_position) { m_positon = a_position; }
	void SetMoment(float a_moment) { m_moment += a_moment; }
	void SetRotation(float a_rot) { m_angularVelocity = a_rot; }
	void SetVelocity(glm::vec2 a_vel) { m_velocity = a_vel; }
	void SetLinearDrag(float a_linearDrag) { m_linearDrag = a_linearDrag; }

	void TriggerEnter(PhysicsObject* a_otherObject);

	float OpposingColour(float a_value);

protected:

	float m_linearDrag = 0.03f;
	float m_angularDrag = 0.03f;

	glm::vec2 m_positon;
	glm::vec2 m_velocity;
	float m_rotation; // In 2D we only need a single float
	float m_mass;
	float m_angularVelocity;
	float m_moment;

	// We will store the local x and y axes of the box based on its angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;

};

