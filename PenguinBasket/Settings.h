#pragma once
#include <vector>
#include <glm/glm.hpp>

class Settings
{
public:
	static int DRAW_CALLS;
	static int FPS;
	static bool Pause;
	static double* Time;
	static double DrawTime;
	static bool Multiplayer;
	static int Ping;
	static unsigned long RamUsage;
	static unsigned long CpuUsage;
	static float WallLightningMultiplier;
	static std::vector < std::pair<int, glm::vec3> > blockSpriteIndices;
};

