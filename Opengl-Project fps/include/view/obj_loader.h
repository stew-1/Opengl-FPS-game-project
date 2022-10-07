#pragma once

/*struct image {
	unsigned char* pixels;
	int width, height, channels;
};*/
#include<vector>
#include <glm/gtc/type_ptr.hpp>

namespace util
{
	std::vector<float> load_model_from_file(const char* filename, glm::mat4 preTransform);
}