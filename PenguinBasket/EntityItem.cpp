#include "EntityItem.h"


EntityItem::EntityItem(std::shared_ptr<Item> item)
{
	this->item = item;
	this->shape->SetSize(0.5, 0.5);
}


void EntityItem::Update(GLFWwindow* window, double deltaTime)
{
	Entity::Update(window, deltaTime);
	if (!Collidable)
		vel *= 0.9f;
}

void EntityItem::OnPlayerCollision()
{
	
}
