#pragma once
#include <GLFW/glfw3.h>
#include "glm\glm.hpp"
#include "Box.h"
#include "Animation.h"

class Entity
{
public:
	Entity();
	~Entity();

	glm::vec2 GetPosition() { return pos; };
	glm::vec2 GetVelocity() { return vel; };
	glm::vec2 GetSize() { return glm::vec2(shape->GetW(), shape->GetH()); };
	char GetLight() { return light; };

	void SetPosition(glm::vec2 pos) { this->pos = pos; };
	void SetVelocity(glm::vec2 vel) { this->vel = vel; };
	void SetVelocityX(float velX) { this->vel.x = velX; isWalking = true; };
	void SetVelocityY(float velY) { this->vel.y = velY; };
	void AddVelocity(glm::vec2 vec) { this->vel += vec; };
	glm::vec2 GetCenter() { return glm::vec2(shape->GetCenterX(), shape->GetCenterY()); };

	void SetLight(char light) { this->light = light; };

	void virtual Update(GLFWwindow* window, double deltaTime);
	void virtual Render();
	void virtual OnPlayerCollision();

	Animation* animation;
	bool onGround;
	bool active;
	glm::vec2 pos;
	glm::vec2 prevPos;
	glm::vec2 vel;
	char light;
	Box* shape;
	bool OnScreen = false;
	bool Collidable;
	bool Gravity;
	int direction = 1;
	bool isWalking = false;
};

