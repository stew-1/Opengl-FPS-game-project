#pragma once
#include <algorithm>
#include "cube.h"
#include "player.h"

class Scene
{
	public:
		Scene();
		~Scene();
		void update(float rate);
		void movePlayer(glm::vec3 dPos);
		void spinPlayer(glm::vec3 dEulers);

		Cube* cube;
		Player* player;
};