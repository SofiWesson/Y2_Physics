#pragma once
#include "PhysicsObject.h"
/* Aone sided objectand it is considered to extend infinitly along both sides of its edgeand backwards from its normal direction */


class RigidBody;

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 a_normal, float a_distToOrigin, glm::vec4 a_colour);
	Plane();
	~Plane();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void MakeGizmo();
	//virtual void Debug();
	virtual void ReSetPosition() {}

	void ResolvePlaneCollision(RigidBody* a_rigidbody, glm::vec2 a_contact);

	glm::vec2 GetNormal() { return m_normal; }
	float GetDistance() { return m_distToOrigin; }
	glm::vec4 GetColour() { return m_colour; }

protected:
	glm::vec2 m_normal;
	float m_distToOrigin;
	glm::vec4 m_colour;

};

