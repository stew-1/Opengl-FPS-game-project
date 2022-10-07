#include <glad/glad.h>
#include <fstream>
#include "view/shader.h"
#include <iostream>
#include <sstream>

unsigned int util::load_shader(const char* vertexFilePath, const char* fragmentFilePath)
{

	std::ifstream fileReader;
	std::stringstream bufferedLines;
	std::string Line;

	fileReader.open(vertexFilePath);
	while(std::getline(fileReader, Line))
	{
		bufferedLines << Line << "\n";
	}
	std::string vertexShaderSource =  bufferedLines.str();
	const char* vertexSrc =  vertexShaderSource.c_str();
	bufferedLines.str("");
	fileReader.close();

	fileReader.open(fragmentFilePath);
	while(std::getline(fileReader, Line))
	{
		bufferedLines << Line << "\n";
	}
	std::string fragmentShaderSource =  bufferedLines.str();
	const char* fragmentSrc =  fragmentShaderSource.c_str();
	bufferedLines.str("");
	fileReader.close();


	//compile the shaders
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//last argument is length NULL indicates that the string is NUL-terminated
	glShaderSource(vertexShader, 1, &vertexSrc, NULL);
	glCompileShader(vertexShader);

	//check error status
	int success;
	char errorLog[1024];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 1024, NULL, errorLog);
		std::cout << "Vertex Shader compilation error:\n" << errorLog << '\n';
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//last argument is length NULL indicates that the string is NUL-terminated
	glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
	glCompileShader(fragmentShader);

	//check error status
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 1024, NULL, errorLog);
		std::cout << "fragment Shader compilation error:\n" << errorLog << '\n';
	}

	//link shaders together to form a program, this will run on our GPU to draw stuff
	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);

	//check error status
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error:\n" << errorLog << '\n';
	}

	//memory was allocated to load source code and compile the shaders,
	//but they aren't needed anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shader;
}