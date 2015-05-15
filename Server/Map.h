#pragma once
#include "IMap.h"

class Map : public IMap
{
public:
	Map(int width, int height);
	~Map();

	int GetBlock(int i, int j);
	int GetBlockId(int i, int j);
	void SetBlock(int i, int j, int id);
	void DestroyBlock(int i, int j);
	int GetWidth() { return m_Width; };
	int GetHeight() { return m_Height; };
	int GetSpawnX() { return spawnX; };
	int GetSpawnY() { return spawnY; };
	int GetTop(int i) { for (int j = 0; j < m_Height; j++) if (GetBlock(i, j) > 0) return j; return 0; };

private:
	int m_Width;
	int m_Height;
	unsigned char** m_Blocks;
	int spawnX;
	int spawnY;

};