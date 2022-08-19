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
#include "view/engine.h"
#include "model/scene.h"

int winWidth;
int winHeight;

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
	//float aspectRatio = (float)winWidth / (float)winHeight;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	
	Engine* renderer = new Engine(winWidth, winHeight);

	//game loop
	while (!glfwWindowShouldClose(window)) {

		processInput(window, scene);

		//update
		scene->update(1.0f);

		glfwPollEvents();

		renderer->render(scene);
		


		glfwSwapBuffers(window);

	}

	//free memory
	delete scene;
	delete renderer;
	glfwTerminate();

	return 0;
}