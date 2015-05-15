#pragma once
#include <string>
#include "Entity.h"

class PlayerMP :
	public Entity
{
public:
	short Id;
	std::string Name = "";

	PlayerMP(short Id, float x, float y);

	void virtual Update(GLFWwindow* window, double deltaTime);
};

