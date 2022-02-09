#include <list>
#include <iostream>
#include <algorithm>
#include <glm/ext.hpp>

#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Circle.h"
#include "Plane.h"
#include "Box.h"
#include "Player.h"

PhysicsScene::PhysicsScene() :
	m_timeStep(0.01f),m_gravity(glm::vec2(0, 0))
{

}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::AddActor(PhysicsObject* a_actor)
{
	m_actors.push_back(a_actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* a_actor)
{
	auto it = std::find(m_actors.begin(), m_actors.end(), a_actor);
	if (it != m_actors.end())
		m_actors.erase(it);
}

void PhysicsScene::Update(float a_dt)
{
	// Update physics at a fixed time step.

	static float accumulatedTime = 0.0f;
	accumulatedTime += a_dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		CheckForCollisions();
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->MakeGizmo();
	}
}

//======================================================================================================
/* Function pointer array for handling out collisions */
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane,  PhysicsScene::Plane2Circle,  PhysicsScene::Plane2Box,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle, PhysicsScene::Circle2Box,
	PhysicsScene::Box2Plane,    PhysicsScene::Box2Circle,    PhysicsScene::Box2Box,
};

void PhysicsScene::CheckForCollisions()
{
	int actorCount = m_actors.size();

	/* We need to check our collisions against all other objects exept this one... */

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* obj1 = m_actors[outer];
			PhysicsObject* obj2 = m_actors[inner];
			int shapeID1 = obj1->GetShapeID();
			int shapeID2 = obj2->GetShapeID();

			/* Using the function pointer */
			int functionIdx = (shapeID1 * SHAPE_COUNT) + shapeID2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				/* check if objects have collided */
				collisionFunctionPtr(obj1, obj2);
			}
		}
	}
}

void PhysicsScene::ApplyContactForces(RigidBody* a_rigidbody1, RigidBody* a_rigidbody2, glm::vec2 a_collosionNorm, float a_pen)
{
	if (a_rigidbody1 && a_rigidbody1->IsTrigger() ||
		a_rigidbody2 && a_rigidbody2->IsTrigger())
		return;

	float rigidbody2Mass = a_rigidbody2 ? a_rigidbody2->GetMass() : INT_MAX;
	float rigidbody1Factor = rigidbody2Mass / (a_rigidbody1->GetMass() + rigidbody2Mass);
	a_rigidbody1->SetPosition(a_rigidbody1->GetPosition() - rigidbody1Factor * a_collosionNorm * a_pen);
	if (a_rigidbody2 != nullptr)
	{
		a_rigidbody2->SetPosition(a_rigidbody2->GetPosition() + (1 - rigidbody1Factor) * a_collosionNorm * a_pen);
	}
}

// =============================================== PLANE ===============================================
bool PhysicsScene::Plane2Plane(PhysicsObject* a_plane, PhysicsObject* a_otherPlane)
{
	return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject* a_plane, PhysicsObject* a_circle)
{
	return Circle2Plane(a_circle, a_plane);
}

bool PhysicsScene::Plane2Box(PhysicsObject* a_plane, PhysicsObject* a_box)
{
	Plane* plane = dynamic_cast<Plane*>(a_plane);
	Box* box = dynamic_cast<Box*>(a_box);

	Player* player = dynamic_cast<Player*>(a_box);
	if (player != nullptr)
		box = (Box*)a_box;

	// When the arguments return true collision test
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;

		glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

		// Check all four corners of the box for if any have touched the plane
		for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
		{
			for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
			{
				// Next, grab the position of the corner in world space
				glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
				float distanceFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());

				// This is the total velocity of the box's points in world space
				glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
				glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() * glm::vec2(-displacement.y, displacement.x);

				// This is the component of the point velocity into the plane
				float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());

				// While our box is penetrating the plane...
				if (distanceFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}

		// We have a hit if greater then 0, typically only 1 to 2 corners will collide
		if (numContacts > 0)
		{
			// 

			plane->ResolvePlaneCollision(box, contact / (float)numContacts);
			return true;
		}
	}

	return false;
}

// =============================================== CIRCLE ===============================================
bool PhysicsScene::Circle2Plane(PhysicsObject* a_circle, PhysicsObject* a_plane)
{
	Circle* circle = dynamic_cast<Circle*>(a_circle);
	Plane* plane = dynamic_cast<Plane*>(a_plane);

	Player* player = dynamic_cast<Player*>(a_circle);
	if (player != nullptr)
		circle = (Circle*)a_circle;

	// if this is successful then test for a collision

	if (circle != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float circleToPlane = glm::dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance();
		float intersection = circle->GetRadius() - circleToPlane;
		float velocityOutOfThePlane = glm::dot(circle->GetVelocity(), plane->GetNormal());
		if (intersection > 0 && velocityOutOfThePlane < 0)
		{
			// We can set objects to respond to a plane collision
			glm::vec2 contact = circle->GetPosition() + (collisionNormal * -circle->GetRadius());
			plane->ResolvePlaneCollision(circle, contact);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Circle2Circle(PhysicsObject* a_circle, PhysicsObject* a_otherCircle)
{
	// Tries to cast the object to circle to circle
	Circle* circle1 = dynamic_cast<Circle*>(a_circle);
	Circle* circle2 = dynamic_cast<Circle*>(a_otherCircle);

	Player* player = dynamic_cast<Player*>(a_circle);
	if (player != nullptr)
		circle1 = (Circle*)a_circle;

	player = dynamic_cast<Player*>(a_otherCircle);
	if (player != nullptr)
		circle2 = (Circle*)a_otherCircle;

	// If successful then test for collision
	if (circle1 != nullptr && circle2 != nullptr)
	{
		// Do the maths to change for the overlap
		float dist = glm::distance(circle1->GetPosition(), circle2->GetPosition());

		// if the circles touch, reseolve the colluision.
		float penetration = circle1->GetRadius() + circle2->GetRadius() - dist;

		if (penetration > 0)
		{
			circle1->ResolveCollision(circle2, .5f * (circle1->GetPosition() + circle2->GetPosition()), nullptr, penetration);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Circle2Box(PhysicsObject* a_circle, PhysicsObject* a_box)
{
	return Box2Circle(a_box, a_circle);
}

// =============================================== BOX ===============================================
bool PhysicsScene::Box2Plane(PhysicsObject* a_box, PhysicsObject* a_plane)
{
	return Plane2Box(a_plane, a_box);
}

bool PhysicsScene::Box2Circle(PhysicsObject* a_box, PhysicsObject* a_circle)
{
	Circle* circle = dynamic_cast<Circle*>(a_circle);
	Box* box = dynamic_cast<Box*>(a_box);

	Player* player = dynamic_cast<Player*>(a_circle);
	if (player != nullptr)
		circle = (Circle*)a_circle;

	player = dynamic_cast<Player*>(a_box);
	if (player != nullptr)
		box = (Box*)a_box;

	if (box != nullptr && circle != nullptr)
	{
		// Transform the circle into the box's coordinate space
		glm::vec2 circlePosWorld = circle->GetPosition() - box->GetPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()), glm::dot(circlePosWorld, box->GetLocalY()));

		/* Then find the closest point to the circle center on the box, do this by clamping the coordinate in the box-space
		   to the box's extents */
		glm::vec2 closestPointOnBox = circlePosBox;
		glm::vec2 extents = box->GetExtents();

		if (closestPointOnBox.x < -extents.x)
			closestPointOnBox.x = -extents.x;
		if (closestPointOnBox.x > extents.x)
			closestPointOnBox.x = extents.x;
		if (closestPointOnBox.y < -extents.y)
			closestPointOnBox.y = -extents.y;
		if (closestPointOnBox.y > extents.y)
			closestPointOnBox.y = extents.y;

		// Finally, convert back to world coordinates
		glm::vec2 closestPointInBoxWorld = box->GetPosition() + closestPointOnBox.x * box->GetLocalX() + closestPointOnBox.y * box->GetLocalY();

		glm::vec2 circleToBox = circle->GetPosition() - closestPointInBoxWorld;

		float penetration = circle->GetRadius() - glm::length(circleToBox);

		if (penetration > 0)
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointInBoxWorld;
			box->ResolveCollision(circle, contact, &direction, penetration);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject* a_box, PhysicsObject* a_otherBox)
{
	Box* box1 = dynamic_cast<Box*>(a_box);
	Box* box2 = dynamic_cast<Box*>(a_otherBox);

	Player* player = dynamic_cast<Player*>(a_box);
	if (player != nullptr)
		box1 = (Box*)a_box;

	player = dynamic_cast<Player*>(a_otherBox);
	if (player != nullptr)
		box2 = (Box*)a_otherBox;

	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 boxPos = box2->GetPosition() - box1->GetPosition();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;

		box1->CheckCorners(*box2, contact, numContacts, pen, norm);
		if (box2->CheckCorners(*box1, contact, numContacts, pen, norm))
		{
			norm = -norm;
		}

		if (pen > 0)
		{
			box1->ResolveCollision(box2, contact / (float)numContacts, &norm, pen);
		}
		return true;
	}

	return false;
}