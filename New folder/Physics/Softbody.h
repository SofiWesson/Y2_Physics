#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

class PhysicsScene;

class Softbody
{
public:
	static void Build(PhysicsScene* a_scene, glm::vec2 a_position, float a_spacing,
		float a_springForce, float a_damping, std::vector<std::string>& a_strings);

};