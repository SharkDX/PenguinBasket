#include "ItemBlock.h"
#include "Map.h"

ItemBlock::ItemBlock(int blockId, int stack)
: Item(*items[1]->clone())
{
	this->blockId = blockId;
	this->name = "Block item";
}

bool ItemBlock::Use(double mouseX, double mouseY)
{
	return Map::map->TryBuildBlock((int) mouseX, (int) mouseY, blockId);;
}

bool ItemBlock::Special(double mouseX, double mouseY)
{
	return false;
}

ItemBlock* ItemBlock::clone() const
{
	return new ItemBlock(*this);
}