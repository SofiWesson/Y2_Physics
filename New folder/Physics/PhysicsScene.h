#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "RigidBody.h"

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* a_actor);
	void RemoveActor(PhysicsObject* a_actor);

	/* This function will update the physics simulation,
	   it calls the update of each actor and updates
	   internally. This will handle collision detection
	   and resolution */
	void Update(float a_dt);

	/* This is called once per frame and handles the rendering
	   of the physics objects. This will let you draw a variety of
	   objects using the Gizmo class and add them to this loop */
	void Draw();

	void SetGravity(const glm::vec2 a_gravity) { m_gravity = a_gravity; }
	glm::vec2 GetGravity() const { return m_gravity; }

	void SetTimeStep(const float a_timeStep) { m_timeStep = a_timeStep; }
	float GetTimeStep() const { return m_timeStep; }

	// float GetDT() const { return m_myDT; }
	// void SetDT(const float a_dt) { m_myDT = a_dt; }

	void CheckForCollisions();
	
	static void ApplyContactForces(RigidBody* a_rigidbody1, RigidBody* a_rigidbody2,
		glm::vec2 a_collosionNorm, float a_pen);

	static bool Plane2Plane(PhysicsObject* a_plane, PhysicsObject* a_otherPlane);
	static bool Plane2Circle(PhysicsObject* a_plane, PhysicsObject* a_circle);
	static bool Plane2Box(PhysicsObject* a_plane, PhysicsObject* a_box);

	static bool Circle2Plane(PhysicsObject* a_circle, PhysicsObject* a_plane);
	static bool Circle2Circle(PhysicsObject* a_circle, PhysicsObject* a_otherCircle);
	static bool Circle2Box(PhysicsObject* a_circle, PhysicsObject* a_box);

	static bool Box2Plane(PhysicsObject* a_box, PhysicsObject* a_plane);
	static bool Box2Circle(PhysicsObject* a_box, PhysicsObject* a_circle);
	static bool Box2Box(PhysicsObject* a_box, PhysicsObject* a_otherBox);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	float m_myDT;
	
	/* Stores all the physics objects in the scene we create
	   as well as functions that will add or remove objects */
	std::vector<PhysicsObject*> m_actors;

};