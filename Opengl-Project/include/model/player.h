#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct PlayerCreateInfo
{
	glm::vec3 position, eulers;
};

class Player
{
	public:
		glm::vec3 position, eulers, up, forwards, right;
	    Player(PlayerCreateInfo* createInfo);
		void update();
};