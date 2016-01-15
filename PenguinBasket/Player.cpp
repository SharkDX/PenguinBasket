#include "Player.h"


Player::Player()
{
	shape->SetSize(0.6f, 1.8f);
	inventory = new Inventory(30);

	animation = new Animation();
	animation->frameCount = 4;
	animation->frameTime = 10;
	animation->lineCount = 4;
	animation->texturePath = "animationTest";
	animation->offsetX = -21;
	animation->offsetY = -7;
}


void Player::Update(GLFWwindow* window, double deltaTime)
{
	hand1.x = hand2.x = -1;
	if (swinging) {
		itemRot += 1.0f * direction;
		if (direction == 1 && itemRot > 90.0f || direction == - 1 && itemRot < -180.0f)
			itemRot = -45.0f;
	}
	if (swinging)
	{
		float rot = (itemRot - 45.0f) * 3.14f / 180.0f;
		hand1 = glm::vec2(cos(rot), sin(rot)) * 0.2f;
		hand2 = glm::vec2(cos(rot), sin(rot)) * 0.2f;
	}
	Entity::Update(window, deltaTime);
	animation->Update();

	if (vel.x > 0)
		direction = 1.0f;
	else if (vel.x < 0)
		direction = -1.0f;

	if (onGround)
	{
		jumpCount = maxJumps;
		animation->currentLine = 0;
		animation->freeze = false;

		if (swinging)
		{
			animation->currentFrame = 0;
			animation->currentLine = 2;
			animation->freeze = true;
		}
		else if (isWalking)
		{
			animation->frameTime = (int) (30.0f / abs(vel.x));
		}
		else
		{
			animation->frameTime = 1000;
			animation->currentFrame = 0;
		}
	}
	else{
		animation->currentLine = 1;
		animation->currentFrame = 0;
		animation->freeze = true;
	}

}
