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

#include "control/game_app.h"

//int winWidth;
//int winHeight;

int main() {
	
	glfwInit(); //init glfw

	GameAppCreateInfo appInfo;
	appInfo.width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width; //set width and height to native resolution
	appInfo.height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
	
	GameApp* app = new GameApp(&appInfo);

	returnCode next_action = returnCode::CONTINUE;
	
	while(next_action == returnCode::CONTINUE) //mainLoop
	{
		next_action = app->mainLoop();
	}
	
	delete app;

	return 0;
}