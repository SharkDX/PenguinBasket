#include "Water.h"

Water::Water() : Entity()
{
	shape->SetSize(1, 1);
}
void Water::Update(GLFWwindow* window, double deltaTime, std::vector<Water*> waters)
{
	Entity::Update(window, deltaTime);
	for (std::vector<Water*>::iterator it = waters.begin(); it != waters.end(); ++it)
	{
		if ((*it) == this)
			continue;
		double d = glm::distance((*it)->GetPosition(), this->GetPosition());
		if (d == 0 || d > 1)
			continue;
		float l = 1 / d;
		(*it)->AddVelocity(9.8f * (float)deltaTime * l * glm::normalize(glm::vec2((*it)->GetPosition() - this->GetPosition())));
	}
}
