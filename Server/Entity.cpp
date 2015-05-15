#include "Entity.h"

Entity::Entity()
{
	pos = vel = glm::vec2();
	active = true;
	Collidable = true;
	Gravity = true;
}


Entity::~Entity()
{
}

void Entity::Update(double deltaTime)
{
	prevPos = pos;
	pos += vel * (float)deltaTime;
	if (Collidable)
		vel.x *= 0.80f;
	if (Gravity && !onGround)
		vel.y += 9.8f * deltaTime;
}

void Entity::OnPlayerCollision()
{

}