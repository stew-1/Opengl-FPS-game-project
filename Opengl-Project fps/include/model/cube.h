#pragma once
#include <glm/gtc/type_ptr.hpp>

struct CubeCreateInfo
{
	glm::vec3 position, eulers;
};

class Cube
{
	public:
		glm::vec3 position, eulers;
		Cube(CubeCreateInfo* createInfo);
		void update(float rate);
};