#pragma once

#include "Entity.h"
#include "Inventory.h"

class Player : 
	public Entity
{
public:
	float reachDistance = 4.0f;
	int maxJumps = 100;
	int jumpCount = maxJumps;
	glm::vec2 hand1;
	glm::vec2 hand2;

	Inventory* inventory;
	int selectedSlot = 0;
	bool inventoryOpen = false;
	glm::vec2 handPos = glm::vec2();
	float itemRot = 0;
	bool swinging = false;

	Player();
	void virtual Update(GLFWwindow* window, double deltaTime);
};

