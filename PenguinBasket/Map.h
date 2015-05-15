#pragma once
#define MAX_LIGHT 15.0f
#include <map>
#include <vector>
#include <time.h>
#include <iostream>
#include <mutex>
#include "Tile.h"
#include "Entity.h"
#include "Noise.h"
#include "Wall.h"
#include "SkyManager.h"
#include "Player.h"
#include "EntityItem.h"
#include "PlayerMP.h"
#include "Blocker.h"
#include "IMap.h"
#include "Block.h"

class Map
{
public:
	static Map* map;

	int Width;
	int Height;

	int minX;
	int minY;
	int maxX;
	int maxY;
	
	bool lightDirty = false;

	Player* player;
	std::vector<Entity*> entities;
	std::vector<EntityItem*> drops;
	std::map<int, int, Blocker*> blockers;
	std::map<int, PlayerMP*> mpPlayers;
	std::mutex mpPlayersMutex;

	int lightUpdated = 0;
	SkyManager* skyManager;

	Map(const int width, const int height, char* mapData = nullptr);
	~Map();

	bool ValidateBlock(int i, int j) { return !(i < 0 || j < 0 || i >= Width || j >= Height); };
	int GetBlock(int i, int j);
	bool IsSolid(int i, int j);
	int GetWall(int i, int j);
	void SetBlock(int i, int j, int id);
	void DestroyBlock(int i, int j);
	bool TryBuildBlock(int i, int j, int id);
	void UpdateBlock(int i, int j) { if(ValidateBlock(i, j)) blocks[i][j].connectionIndex = GetConnectionIndex(i, j); };
	void UpdateNearBlocks(int i, int j);
	char GetConnectionIndex(int i, int j, bool wall = false);
	void UpdateLight(int i, int j, float level, int previ = -1, int prevj = -1, bool recalculate = false);
	void CalculateLights(bool firstTime = false);
	float GetLight(int i, int j) { if (!ValidateBlock(i, j)) return 0; /*return (float) lightMap[i][j] / 15.0f;*/ return glm::pow(0.8f, MAX_LIGHT - (float) (blocks[i][j].lightValue)); };
	float GetAvgLight(int i, int j);
	int* heightMap;
	Tile** blocks;
	Wall** walls;
	void Update();
	void CelluarAuto(int minlevel, int it);
	int NearLive(int i, int j, int size);
	int GetTop(int i) { for (int j = 0; j < Height; j++) if (GetBlock(i, j) > 0) return j; return 0; };
	int GenerateVein(int i, int j, int id, int size);
	void HandleCollision();
	void HandleEvents();
	void Respawn();
	bool IntersectsEntities(Box* box);
};

