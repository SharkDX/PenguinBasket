#pragma once
#include "Material.h"
#include "ItemBlock.h"

class Block
{
public:
	static Block* Dirt;
	static Block* Stone;
	static Block* GetBlockById(char id);

	char Id;

	Block(char id);
	Block(char id, float hardness);
	Block(char id, float hardness, int coord);
	Block(char id, float hardness, int coord, char lightemit);
	Block(char id, float hardness, int coord, char lightemit, bool solid);
	Block(char id, float hardness, int coord, char lightemit, bool solid, Material material);

	Block* SetTextureCoord(int coord);
	Block* SetSolid(bool solid);
	Block* SetLightEmit(char value);
	Block* SetHardness(float value);
	Block* SetMaterial(Material value);
	
	bool IsSolid();
	int GetTextureCoord();
	char GetLightEmit();
	float GetHardness();
	Material GetMaterial();
	std::shared_ptr<Item> GetDrop();

private:
	static Block* blocks[256];

	int textureCoord = 0; // The number of the block's texture.
	bool solid = true; // Is the block solid, can u pass through.
	char lightEmit = 0; // The light value the block is emitting, from 0 to 15.
	float hardness = 1.0f; // The number of seconds it takes to destroy the block with empty hands.
	Material material = Material::Soil;
};

