#include "Map.h"

Map::Map(int width, int height)
{
	m_Width = width;
	m_Height = height;

	/* Allocating the array */

	m_Blocks = new unsigned char*[m_Width];

	for (int i = 0; i < m_Width; i++)
		m_Blocks[i] = new unsigned char[m_Height];
	
	/* Filling the array with data */

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
			m_Blocks[i][j] = 0;
	}

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = m_Height / 2; j < m_Height; j++)
			m_Blocks[i][j] = 3;
	}

	spawnX = width / 2;
	spawnY = GetTop(spawnX) - 4;
}

int Map::GetBlock(int i, int j)
{
	return m_Blocks[i][j];
}

void Map::SetBlock(int i, int j, int id)
{
	m_Blocks[i][j] = id;
}

void Map::DestroyBlock(int i, int j)
{
	m_Blocks[i][j] = 0;
}
