#include "PlayerMP.h"

PlayerMP::PlayerMP(short Id, float x, float y)
{
	this->Id = Id;
	SetPosition(glm::vec2(x, y));
	shape->SetSize(0.6f, 1.8f);

	animation = new Animation();
	animation->frameCount = 4;
	animation->frameTime = 10;
	animation->lineCount = 4;
	animation->texturePath = "animationTest";
	animation->offsetX = -21;
	animation->offsetY = -7;
}


void PlayerMP::Update(GLFWwindow* window, double deltaTime)
{
	Entity::Update(window, deltaTime);
	animation->Update();

	if (vel.x > 0)
		direction = 1;
	else if (vel.x < 0)
		direction = -1;

	if (onGround) {
		animation->currentLine = 0;
		animation->freeze = false;

		if (abs(vel.x) > 0)
		{
			animation->frameTime = (int) (30.0f / abs(vel.x));
		}
		else
		{
			animation->frameTime = 1000;
			animation->currentFrame = 0;
		}
	}
	else
	{
		animation->currentLine = 1;
		animation->currentFrame = 0;
		animation->freeze = true;
	}
}
