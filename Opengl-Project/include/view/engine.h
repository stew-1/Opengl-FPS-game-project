#pragma once

#include"model/scene.h"
#include"shader.h"
#include"rectangle_model.h"
#include"material.h"
#include <glm/gtx/euler_angles.hpp>

class Engine
{
	public:
	Engine(int width, int height);
	~Engine();
	
	void createMaterials();
	void createModels();
	void render(Scene* scene);
	
	unsigned int shader;
	Material* woodMaterial;
	RectangleModel* cubeModel;
};