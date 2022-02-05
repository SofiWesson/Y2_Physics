#include <list>
#include <iostream>

#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Circle.h"
#include "Plane.h"
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
	PhysicsScene::Plane2Plane,  PhysicsScene::Plane2Circle,  PhysicsScene::Plane2Player,   PhysicsScene::Plane2Box,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle, PhysicsScene::Circle2Player, PhysicsScene::Circle2Box,
	PhysicsScene::Player2Plane, PhysicsScene::Player2Circle, PhysicsScene::Player2Player, PhysicsScene::Player2Box,
	PhysicsScene::Box2Plane,    PhysicsScene::Box2Circle,    PhysicsScene::Box2Player,    PhysicsScene::Box2Box,
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

bool PhysicsScene::Plane2Plane(PhysicsObject* a_plane, PhysicsObject* a_otherPlane)
{
	return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject* a_plane, PhysicsObject* a_circle)
{
	return Circle2Plane(a_circle, a_plane);
}

bool PhysicsScene::Plane2Player(PhysicsObject* a_plane, PhysicsObject* a_player)
{
	Plane* plane = dynamic_cast<Plane*>(a_plane);
	Player* player = dynamic_cast<Player*>(a_player);

	// if this is successful then test for a collision

	if (player != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float circleToPlane = glm::dot(player->GetPosition(), plane->GetNormal()) - plane->GetDistance();
		float intersection = player->GetRadius() - circleToPlane;
		float velocityOutOfThePlane = glm::dot(player->GetVelocity(), plane->GetNormal());
		if (intersection > 0 && velocityOutOfThePlane < 0)
		{
			// We can set the circles respones
			player->ApplyForce(-player->GetVelocity() * player->GetMass(), player->GetPosition());
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Plane2Box(PhysicsObject* a_plane, PhysicsObject* a_box)
{
	return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject* a_circle, PhysicsObject* a_plane)
{
	Circle* circle = dynamic_cast<Circle*>(a_circle);
	Plane* plane = dynamic_cast<Plane*>(a_plane);

	// if this is successful then test for a collision

	if (circle != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float circleToPlane = glm::dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance();
		float intersection = circle->GetRadius() - circleToPlane;
		float velocityOutOfThePlane = glm::dot(circle->GetVelocity(), plane->GetNormal());
		if (intersection > 0 && velocityOutOfThePlane < 0)
		{
			// We can set the circles respones
			circle->ApplyForce(-circle->GetVelocity() * circle->GetMass(), circle->GetPosition());
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

	// If successful then test for collision
	if (circle1 != nullptr && circle2 != nullptr)
	{
		// Do the maths to change for the overlap
		float dist = glm::distance(circle1->GetPosition(), circle2->GetPosition());

		// if the circles touch, reseolve the colluision.
		float penetration = circle1->GetRadius() + circle2->GetRadius() - dist;

		if (penetration > 0)
		{
			circle1->ResolveCollision(circle2, .5f * (circle1->GetPosition() + circle2->GetPosition()));

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Circle2Player(PhysicsObject* a_circle, PhysicsObject* a_player)
{
	// Tries to cast the object to circle to circle
	Circle* circle = dynamic_cast<Circle*>(a_circle);
	Player* player = dynamic_cast<Player*>(a_player);

	// If successful then test for collision
	if (circle != nullptr && a_player != nullptr)
	{
		// Do the maths to change for the overlap
		float dist = glm::distance(circle->GetPosition(), player->GetPosition());

		// if the circles touch, reseolve the colluision.
		float penetration = circle->GetRadius() + player->GetRadius() - dist;

		if (penetration > 0)
		{
			circle->ResolveCollision(player, .5f * (circle->GetPosition() + player->GetPosition()));

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Circle2Box(PhysicsObject* a_circle, PhysicsObject* a_box)
{
	return false;
}

bool PhysicsScene::Player2Plane(PhysicsObject* a_player, PhysicsObject* a_plane)
{
	return Plane2Player(a_plane, a_player);
}

bool PhysicsScene::Player2Circle(PhysicsObject* a_player, PhysicsObject* a_circle)
{
	return Circle2Player(a_circle, a_player);
}

bool PhysicsScene::Player2Player(PhysicsObject* a_player, PhysicsObject* a_otherPlayer)
{
	// Tries to cast the object to circle to circle
	Player* player1 = dynamic_cast<Player*>(a_player);
	Player* player2 = dynamic_cast<Player*>(a_otherPlayer);

	// If successful then test for collision
	if (player1 != nullptr && player2 != nullptr)
	{
		// Do the maths to change for the overlap
		float dist = glm::distance(player1->GetPosition(), player2->GetPosition());

		// if the circles touch, reseolve the colluision.
		float penetration = player1->GetRadius() + player2->GetRadius() - dist;

		if (penetration > 0)
		{
			player1->ResolveCollision(player2, .5f * (player1->GetPosition() + player2->GetPosition()));

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Player2Box(PhysicsObject* a_player, PhysicsObject* a_box)
{
	return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject* a_box, PhysicsObject* a_plane)
{
	return false;
}

bool PhysicsScene::Box2Circle(PhysicsObject* a_box, PhysicsObject* a_circle)
{
	return false;
}

bool PhysicsScene::Box2Player(PhysicsObject* a_box, PhysicsObject* a_player)
{
	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject* a_box, PhysicsObject* a_otherBox)
{
	return false;
}
