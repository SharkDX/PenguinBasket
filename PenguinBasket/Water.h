#pragma once
#include "Entity.h"
#include <vector>

class Water :
	public Entity
{
public:
	Water();
	void virtual Update(GLFWwindow* window, double deltaTime, std::vector<Water*> waters);
};

