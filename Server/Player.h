#pragma once

#include "Entity.h"

class Player :
	public Entity
{
public:
	//Inventory* inventory;
	int selectedSlot = 0;
	bool inventoryOpen = false;

	Player();
	void virtual Update(double deltaTime);
};

