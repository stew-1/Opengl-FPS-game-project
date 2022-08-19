//END OF SERIES OPTIMIZATION:
//batching
//enable backface culling
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "view/shader.h"
#include "view/rectangle_model.h"
#include "view/image.h"
#include "model/scene.h"

int winWidth;
int winHeight;

float nearDisplay = 0.1f;
float farDisplay = 100.0f;

GLFWwindow* initialize() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	winWidth = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	winHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "This is working I hope", glfwGetPrimaryMonitor(), NULL);
	if (!window) {
		std::cout << "Window creation failed\n";
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD initialization failed\n";
		return NULL;
	}

	glViewport(0, 0, winWidth, winHeight);

	return window;
}

void processInput(GLFWwindow* window, Scene* scene) {

	int wasdState{0};
	float walk_direction{scene->player->eulers.z};
	bool walking{false};
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		wasdState += 1;
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		wasdState += 2;
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		wasdState += 4;
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		wasdState += 8;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//switch on wasd state
	switch (wasdState)
	{
	case 1:
	case 11:
		//forwards
		walking = true;
		break;
	case 3:
		//left-forwards
		walking = true;
		walk_direction += 45;
		break;
	case 2:
	case 7:
		//left
		walking = true;
		walk_direction += 90;
		break;
	case 6:
		//left-backwards
		walking = true;
		walk_direction += 135;
		break;
	case 4:
	case 14:
		//backwards
		walking = true;
		walk_direction += 180;
		break;
	case 12:
		//right-backwards
		walking = true;
		walk_direction += 225;
		break;
	case 8:
	case 13:
		//right
		walking = true;
		walk_direction += 270;
		break;
	case 9:
		//right-forwards
		walking = true;
		walk_direction += 315;
	}
		if(walking)
		{
			scene->movePlayer(0.01f * glm::vec3{
				glm::cos(glm::radians(walk_direction)),
				glm::sin(glm::radians(walk_direction)),
				0.0f
			});
		}

		//mouse
		double mouse_x, mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		glfwSetCursorPos(window, (float)winWidth/2, (float)winHeight/2);

		float delta_x{static_cast<float>(mouse_x - (float)winWidth/2)};
		float delta_y{static_cast<float>(mouse_y - (float)winHeight/2)};

		scene->spinPlayer(
					{
						0.0f, delta_y, -delta_x
					}
		);
}

int main() {

	Scene* scene = new Scene();

	GLFWwindow* window = initialize();
	if (!window) {
		glfwTerminate();
		return -1;
	}
	float aspectRatio = (float)winWidth / (float)winHeight;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	unsigned int shader = util::load_shader("shaders/vertex.txt", "shaders/fragment.txt");
	//nine out of ten unexpected errors in OpenGL are caused by not using the right program.
	glUseProgram(shader);
	
	RectangleModelCreateInfo cubeInfo;
	cubeInfo.size = {2.0f, 1.0f, 1.0f};
	
	RectangleModel* cubeModel = new RectangleModel(&cubeInfo);

	//texture
	glUniform1i(glGetUniformLocation(shader, "basicTexture"), 0);

	//load image
	int texWidth, texHeight;
	
	image material = util::load_from_file("textures/wood.jpeg");
	
	texWidth = material.width;
	texHeight = material.height;
	
	unsigned char* data = material.pixels;
	unsigned int texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_RGBA8, texWidth, texHeight);
	glTextureSubImage2D(texture, 0, 0, 0, texWidth, texHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	util::free_image_memory(material);

	//set up framebuffer
	glClearColor(0.5f, 0.1f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glm::mat4 projectionTransform = glm::perspective(45.0f, aspectRatio, nearDisplay, farDisplay);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projectionTransform));

	//game loop
	while (!glfwWindowShouldClose(window)) {

		processInput(window, scene);

		//update
		scene->update(1.0f);

		//prepare shaders
		glm::mat4 viewTransform{glm::lookAt(scene->player->position, scene->player->position + scene->player->forwards, scene->player->up)};
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(viewTransform));

		glfwPollEvents();

		//update transform
		//float angle = glm::radians(static_cast<float>(10*glfwGetTime()));
		glm::mat4 modelTransform = glm::mat4(1.0f);
		modelTransform = glm::translate(modelTransform, scene->cube->position);

		modelTransform = modelTransform * glm::eulerAngleXYZ(scene->cube->eulers.x, scene->cube->eulers.y, scene->cube->eulers.z);

		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(modelTransform));

		//draw

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader);

		glBindTextureUnit(0, texture);

		glBindVertexArray(cubeModel->VAO);
		glDrawArrays(GL_TRIANGLES, 0, cubeModel->vertexCount);

		glfwSwapBuffers(window);

	}

	//free memory
	delete cubeModel;
	glDeleteTextures(1, &texture);
	glDeleteProgram(shader);
	glfwTerminate();

	return 0;
}