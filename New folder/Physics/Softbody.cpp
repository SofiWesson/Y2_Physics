#include "Softbody.h"
#include "PhysicsScene.h"
#include "Circle.h"
#include "Spring.h"

void Softbody::Build(PhysicsScene* a_scene, glm::vec2 a_position, float a_spacing, float a_springForce, float a_damping, std::vector<std::string>& a_strings)
{
	int numCols = a_strings.size();
	int numRows = a_strings[0].length();

	/* We want to traverse across the array and add circles where the correct ASCII art states they should be */

	// This is used to create the particles as our first pass, based on the ASCII art provided
	Circle** circles = new Circle * [numRows * numCols];

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (a_strings[j][i] == '0')
			{
				circles[i * numCols + j] = new Circle(a_position + glm::vec2(i, j) * a_spacing,
					glm::vec2(0, 0), 1, 2, glm::vec4(.5f, .5f, .5f, 1));
				a_scene->AddActor(circles[i * numCols + j]);
			}
			else
			{
				circles[i * numCols + j] = nullptr;
			}
		}
	}

	// This is the second pass, we will connect the particles with springs
	for (int i = 1; i < numRows; i++)
	{
		for (int j = 1; j < numCols; j++)
		{
			Circle* p11 = circles[i * numCols + j];
			Circle* p01 = circles[(i - 1) * numCols + j];
			Circle* p10 = circles[i * numCols + j - 1];
			Circle* p00 = circles[(i - 1) * numCols + j - 1];

			// Make the springs connect to their cardinal neighbours if they exist
			if (p11 && p01) a_scene->AddActor(new Spring(p11, p01, a_damping, a_springForce));
			if (p11 && p10) a_scene->AddActor(new Spring(p11, p10, a_damping, a_springForce));
			if (p10 && p00) a_scene->AddActor(new Spring(p10, p00, a_damping, a_springForce));
			if (p01 && p00) a_scene->AddActor(new Spring(p01, p00, a_damping, a_springForce));
			
			// Make the diagonal springs connect if the correct partner exists
			if (p11 && p00) a_scene->AddActor(new Spring(p11, p00, a_damping, a_springForce));
			if (p01 && p10) a_scene->AddActor(new Spring(p01, p10, a_damping, a_springForce));

			bool endOfJ = j == numCols - 1;
			bool endOfI = i == numRows - 1;

			Circle* p22 = (!endOfI && !endOfJ) ? circles[(i + 1) * numCols + (j + 1)] : nullptr;
			Circle* p02 = !endOfJ ? circles[(i - 1) * numCols + (j + 1)] : nullptr;
			Circle* p20 = !endOfI ? circles[(i + 1) * numCols + j - 1] : nullptr;
			
			if (p22 && p02) a_scene->AddActor(new Spring(p22, p02, a_damping, a_springForce));
			if (p22 && p20) a_scene->AddActor(new Spring(p22, p20, a_damping, a_springForce));
			if (p20 && p00) a_scene->AddActor(new Spring(p20, p00, a_damping, a_springForce));
			if (p02 && p00) a_scene->AddActor(new Spring(p02, p00, a_damping, a_springForce));
		}
	}
}