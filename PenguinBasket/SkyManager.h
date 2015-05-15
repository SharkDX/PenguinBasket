#pragma once
#include <GLFW\glfw3.h>
#include "Cloud.h"
#include <vector>
#include "Profiler.h"
#include <algorithm>
#include <time.h>

struct Rain
{
	glm::vec2 pos;
	char size;
};

class SkyManager
{
public:
	double Time = 0;
	unsigned int lastTimeAdd = 0;
	bool isRaining = false;

	glm::vec3 skyColor = glm::vec3(0.4f, 0.6f, 1.0f);
	char lightLevel = 0;
	int MapWidth;
	float SkySpeed = 0.002f;
	int CloudCount = 30;
	const static int rainH = 10;
	const static int rainW = 100;

	Rain* rain[rainW * rainH];
	std::vector<Cloud*> clouds;

	SkyManager(int mapWidth);
	void Update();
};

