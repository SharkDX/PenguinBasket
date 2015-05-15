#pragma once
#include "Item.h"

class ItemBlock :
	public Item
{
public:
	int blockId;

	ItemBlock(int blockId, int stack);
	bool Use(double mouseX, double mouseY);
	bool Special(double mouseX, double mouseY);
	virtual ItemBlock* clone() const;
};

