#pragma once
#include "glm\glm.hpp"

class Entity
{
public:
	Entity();
	~Entity();

	glm::vec2 GetPosition() { return pos; };
	glm::vec2 GetVelocity() { return vel; };

	void SetPosition(glm::vec2 pos) { this->pos = pos; };
	void SetVelocity(glm::vec2 vel) { this->vel = vel; };
	void SetVelocityX(float velX) { this->vel.x = velX; };
	void SetVelocityY(float velY) { this->vel.y = velY; };
	void AddVelocity(glm::vec2 vec) { this->vel += vec; };

	void virtual Update(double deltaTime);
	void virtual OnPlayerCollision();

	bool onGround;
	bool active;
	glm::vec2 pos;
	glm::vec2 prevPos;
	glm::vec2 vel;
	bool Collidable;
	bool Gravity;
};

