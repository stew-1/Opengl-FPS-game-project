#include "model/cube.h"

Cube::Cube(CubeCreateInfo* createInfo)
{
	this->position = createInfo->position;
	this->eulers = createInfo->eulers;
}

void Cube::update(float rate)
{
	eulers.x += 0.001 * rate;
	if(eulers.x > 360)
	{
		eulers.x -= 360;
	}
	eulers.y += 0.002 * rate;
	if(eulers.y > 360)
	{
			eulers.y -= 360;
	}
}