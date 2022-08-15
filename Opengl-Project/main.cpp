#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "view/shader.h"
#include "model/scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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


	//triangle data, using vector type to store a list of floats
	// x,y,z,s,t
	std::vector<float> vertices = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,

			 0.5f,  0.5f, -0.5f, 1.0f, 1,
			-0.5f,  0.5f, -0.5f, 0.0f, 1,
			-0.5f, -0.5f, -0.5f, 0.0f, 0,

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, //top
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, //left
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, //right
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //back
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, //front
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
	};

	int vertexCount = vertices.size()/5;

	//this data exists on the cpu side, we need to get it to the gpu side.
	//This is done with a vertex buffer object (vbo)
	unsigned int VBO;
	glCreateBuffers(1, &VBO);

	//a vertex array object stores data and attribute information
	unsigned int VAO;
	glCreateVertexArrays(1, &VAO);
	//bind the vertex buffer object to the VAO
	//(VAO, binding index, VBO, offset, stride)
	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));

	//send data to our new VBO. memory is allocated at this point (VBO will have to be deleted later)
	//(VBO, size in bytes, pointer to data start, mode)
	glNamedBufferStorage(VBO, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
	/*
		DYNAMIC_STORAGE: data may be updated after creation through glBufferSubData()
		MAP_READ: data can be read
		MAP_WRITE: data can be written to
		MAP_PERSISTENT: data can be worked with by the GPU while mapped (taking CPU instructions like read/write)
		MAP_COHERENT: read/write instructions are executed immediately
		CLIENT_STORAGE: store buffer data on client (CPU) side
	*/

	/*
		the data has been loaded into the GPU's vram (video ram),
		however the GPU doesn't know how to use it.
		Where is the position data, where is the color data?
		We need to create attribute pointers to describe how to get the attributes out
		of that list of big, dumb numbers.
	*/
	//pos
	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(VAO, 0, 0);

	//texture coords
	glEnableVertexArrayAttrib(VAO, 1);
	glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
	glVertexArrayAttribBinding(VAO, 1, 0);

	//texture
	glUniform1i(glGetUniformLocation(shader, "basicTexture"), 0);

	//load image
	int texWidth, texHeight, channelCount;
	unsigned char* data = stbi_load("textures/wood.jpeg", &texWidth, &texHeight, &channelCount, STBI_rgb_alpha); //change to STBI_rgb
	unsigned int texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_RGBA8, texWidth, texHeight);
	glTextureSubImage2D(texture, 0, 0, 0, texWidth, texHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

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

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glfwSwapBuffers(window);

	}

	//free memory
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shader);
	glfwTerminate();

	return 0;
}