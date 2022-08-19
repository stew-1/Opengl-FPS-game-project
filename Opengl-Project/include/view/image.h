#pragma once

struct image {
	unsigned char* pixels;
	int width, height, channels;
};

namespace util
{
	image load_from_file(const char* filename);
	
	void free_image_memory(image oldImage);
}