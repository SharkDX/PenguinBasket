#pragma once
#include "Item.h"
class ItemTool :
	public Item
{
public:
	ItemTool();
	bool Use(double mouseX, double mouseY);
	bool Special(double mouseX, double mouseY);
	virtual ItemTool* clone() const;
};

