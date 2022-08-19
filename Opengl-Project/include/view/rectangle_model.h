#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

struct RectangleModelCreateInfo
{
	glm::vec3 size;
};

class RectangleModel
{
	public:
		unsigned int VBO, VAO, vertexCount;
		std::vector<float> vertices;
		
	    RectangleModel(RectangleModelCreateInfo* createInfo);
		~RectangleModel();
};