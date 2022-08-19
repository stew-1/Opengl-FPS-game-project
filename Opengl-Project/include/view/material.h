#pragma once
#include "image.h"
#include <glad/glad.h>

struct MaterialCreateInfo
{
	const char* filename;
};

class Material
{
	public:
		unsigned int texture;
		
	    Material(MaterialCreateInfo* createInfo);
		~Material();
		void use();
};