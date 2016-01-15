#include "Settings.h"


int Settings::DRAW_CALLS = 0;
int Settings::FPS = 0;
double* Settings::Time = 0;
bool Settings::Pause = false;
double Settings::DrawTime = 0;
bool Settings::Multiplayer = false;
int Settings::Ping = 0;
unsigned long Settings::RamUsage = 0;
unsigned long Settings::CpuUsage = 0;
float Settings::WallLightningMultiplier = 0.5f;

// "bit - position - sprite_index" table.
// The is a texture for each block that represents:
// 0 1 0
// 8 0 2
// 0 4 0
// The center is the block itself, and around it the edges.
// For example: 4, (0, +1), 7.0f
// if (bit 4 is ON in the connections_variable) then
//		we should draw the edge in sprite index 7 in the position (0, +1) relative to the block position.
//
std::vector < std::pair<int, glm::vec3> > Settings::blockSpriteIndices {
	std::make_pair(1, glm::vec3(0, -1, 1.0f)),
	std::make_pair(2, glm::vec3(+1, 0, 5.0f)),
	std::make_pair(4, glm::vec3(0, +1, 7.0f)),
	std::make_pair(8, glm::vec3(-1, 0, 3.0f))
};