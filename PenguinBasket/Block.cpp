#include "Block.h"

Block* Block::blocks[256];
Block* Block::Stone = new Block(1, 1.0f, 2);
Block* Block::Dirt = new Block(2, 1.0f, 3);

Block* Block::GetBlockById(char id)
{
	return blocks[id];
}

Block::Block(char id) : Block(id, 1.0f, id, 0, true)
{
}

Block::Block(char id, float hardness) : Block(id, hardness, id, 0, true)
{

}

Block::Block(char id, float hardness, int coord) : Block(id, hardness, coord, 0, true)
{

}

Block::Block(char id, float hardness, int coord, char lightemit) : Block(id, hardness, coord, lightemit, true, Material::Soil)
{

}

Block::Block(char id, float hardness, int coord, char lightemit, bool solid) : Block(id, hardness, coord, lightemit, solid, Material::Soil)
{

}

Block::Block(char id, float hardness, int coord, char lightemit, bool solid, Material material) 
{
	this->Id = id;
	this->hardness = hardness;
	this->textureCoord = coord;
	this->lightEmit = lightemit;
	this->solid = solid;
	this->material = material;
	this->blocks[id] = this;
}


Block* Block::SetTextureCoord(int coord)
{
	textureCoord = coord;
	return this;
}

Block* Block::SetSolid(bool solid)
{
	this->solid = solid;
	return this;
}

Block* Block::SetLightEmit(char value)
{
	lightEmit = value;
	return this;
}

Block* Block::SetHardness(float value)
{
	hardness = value;
	return this;
}

Block* Block::SetMaterial(Material value)
{
	material = value;
	return this;
}

bool Block::IsSolid()
{
	return solid;
}

int Block::GetTextureCoord()
{
	return textureCoord;
}

char Block::GetLightEmit()
{
	return lightEmit;
}

float Block::GetHardness()
{
	return hardness;
}

std::shared_ptr<Item> Block::GetDrop()
{
	return std::shared_ptr<Item>(new ItemBlock(Id, 1));
}