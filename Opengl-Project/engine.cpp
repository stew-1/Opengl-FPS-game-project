#include"view/engine.h"

Engine::Engine(int width, int height)
{
	float aspectRatio = (float)width / (float)height;
	
	float nearDisplay = 0.1f;
	float farDisplay = 100.0f;
	
	shader = util::load_shader("shaders/vertex.txt", "shaders/fragment.txt");
	//nine out of ten unexpected errors in OpenGL are caused by not using the right program.
	glUseProgram(shader);
	
	glUniform1i(glGetUniformLocation(shader, "basicTexture"), 0);
	
	//set up framebuffer
	glClearColor(0.5f, 0.1f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glm::mat4 projectionTransform = glm::perspective(45.0f, aspectRatio, nearDisplay, farDisplay);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projectionTransform));
	createModels();
	createMaterials();
}
	
	
	
Engine::~Engine()
{
	delete cubeModel;
	delete woodMaterial;
	glDeleteProgram(shader);
}

void Engine::render(Scene* scene)
{
	//prepare shaders
	glm::mat4 viewTransform{glm::lookAt(scene->player->position, scene->player->position + scene->player->forwards, scene->player->up)};
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(viewTransform));
	
	glm::mat4 modelTransform = glm::mat4(1.0f);
		modelTransform = glm::translate(modelTransform, scene->cube->position);

		modelTransform = modelTransform * glm::eulerAngleXYZ(scene->cube->eulers.x, scene->cube->eulers.y, scene->cube->eulers.z);

		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(modelTransform));
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader);
		
		woodMaterial->use();

		glBindVertexArray(cubeModel->VAO);
		glDrawArrays(GL_TRIANGLES, 0, cubeModel->vertexCount);
}
	
void Engine::createMaterials()
{
	MaterialCreateInfo materialInfo;
	materialInfo.filename = "textures/wood.jpeg";
	
	woodMaterial = new Material(&materialInfo);
}
void Engine::createModels()
{
	RectangleModelCreateInfo cubeInfo;
	cubeInfo.size = {2.0f, 1.0f, 1.0f};
	cubeModel = new RectangleModel(&cubeInfo);
}
void Engine::render(Scene* scene);