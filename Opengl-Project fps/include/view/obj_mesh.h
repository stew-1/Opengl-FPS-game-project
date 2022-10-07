#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "obj_loader.h"

struct MeshCreateInfo
{
	const char* filename;
	glm::mat4 preTransform;

};

class ObjMesh
{
	public:
		unsigned int VBO, VAO, vertexCount;
		
	    ObjMesh(MeshCreateInfo* createInfo);
		~ObjMesh();
};