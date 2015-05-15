#include "Item.h"
#include "ItemTool.h"
#include "Map.h"

Item* Item::items[256];
Item* Item::pickaxe;
Item* Item::ingot;

void Item::InitItems()
{
	Item* itemBlock = new Item();
	itemBlock->Id = 1;
	itemBlock->maxStack = 99;
	itemBlock->consumable = true;
	itemBlock->continuous = true;
	itemBlock->block = true;
	itemBlock->useTime = 10;
	items[itemBlock->Id] = itemBlock;

	pickaxe = new ItemTool();
	pickaxe->Id = 2;
	pickaxe->name = "Wooden Pickaxe";
	pickaxe->textureIndex = 0;
	items[pickaxe->Id] = pickaxe;

	ingot = new Item();
	ingot->Id = 3;
	ingot->name = "Iron Ingot";
	ingot->textureIndex = 1;
	ingot->maxStack = 99;
	ingot->consumable = false;
	ingot->continuous = false;
	items[ingot->Id] = ingot;
}

Item::Item()
{
}

Item::~Item()
{
}

bool Item::Use(double mouseX, double mouseY)
{
	return true;
}

bool Item::Special(double mouseX, double mouseY)
{
	return true;
}

Item* Item::clone() const
{
	return new Item(*this);
}