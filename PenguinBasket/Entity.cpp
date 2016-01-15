#include "Entity.h"

Entity::Entity()
{
	animation = NULL;
	pos = vel = glm::vec2();
	shape = new Box(0, 0, 0.6f, 1.8f);
	light = 0;
	active = true;
	Collidable = true;
	Gravity = true;
}


Entity::~Entity()
{
	if (shape)
		delete shape;
	if (animation)
		delete animation;
}

void Entity::Update(GLFWwindow* window, double deltaTime)
{
	vel += force;
	force = glm::vec2();
	prevPos = pos;
	pos += vel * (float)deltaTime;
	if (Collidable)
		vel.x *= 0.80f;
	if (Gravity && !onGround)
		vel.y += 9.8f * deltaTime;
}

void Entity::Render()
{
	
}

void Entity::OnPlayerCollision()
{

}