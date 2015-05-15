#include "ItemTool.h"
#include "Map.h"

ItemTool::ItemTool()
: Item()
{
	this->consumable = false;
	this->continuous = true;
	this->maxStack = 1;
	this->useTime = 10;
	this->block = false;
}

bool ItemTool::Use(double mouseX, double mouseY)
{
	int blockId = Map::map->GetBlock((int) mouseX, (int) mouseY);
	if (blockId == 0)
		return false;

	Map::map->DestroyBlock((int) mouseX, (int) mouseY);
	return true;
}

bool ItemTool::Special(double mouseX, double mouseY)
{
	return false;
}

ItemTool* ItemTool::clone() const
{
	return new ItemTool(*this);
}