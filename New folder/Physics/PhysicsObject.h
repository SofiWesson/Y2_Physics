#pragma once
#include <glm/glm.hpp>
#include <functional>
#include <list>

/* This is a pure abstract class. It will be used as a base for
   the derived classes that will use FixedUpdate */

enum ShapeType
{
	JOINT = -1,
	PLANE = 0,
	CIRCLE,
	BOX,
	SHAPE_COUNT
};

enum BallType
{
	SOLID,
	STRIPES,
	EIGHTBALL,
	CUEBALL
};

class PhysicsObject
{
public:
	void Update();
	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep) = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {};
	void Debug();

	ShapeType GetShapeID() { return m_shapeID; }
	float GetElasticity() { return m_elasticity; }
	bool IsTrigger() { return m_isTrigger; }
	bool CanCollide() { return m_canCollide; }

	void SetElasticity(float a_elsticity) { m_elasticity = a_elsticity; }
	void SetTrigger(bool a_trigger) { m_isTrigger = a_trigger; }
	void SetCollider(bool a_canCollder) { m_canCollide = a_canCollder; }

	std::function<void(PhysicsObject*)> triggerEnter;
	std::function<void(PhysicsObject*)> triggerExit;

	std::list<PhysicsObject*> m_objectsInside;
	std::list<PhysicsObject*> m_objectsInsideThisFrame;

protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

	ShapeType m_shapeID;
	float m_elasticity;
	bool m_isKinematic;
	bool m_isTrigger;
	bool m_canCollide;
};