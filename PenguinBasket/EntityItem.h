#pragma once
#include "Entity.h"
#include "Item.h"

class EntityItem :
	public Entity
{
public:
	std::shared_ptr<Item> item;

	EntityItem(std::shared_ptr<Item> item);
	void virtual Update(GLFWwindow* window, double deltaTime);
	void virtual OnPlayerCollision();
};

