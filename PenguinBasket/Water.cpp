#include "Water.h"

Water::Water() : Entity()
{
	shape->SetSize(1.0, 1.0);
}
void Water::Update(GLFWwindow* window, double deltaTime, std::vector<Water*> waters)
{
	float target_distance = 0.5f;
	for (std::vector<Water*>::iterator it = waters.begin(); it != waters.end(); ++it)
	{
		if ((*it) == this || (*it)->GetPosition().x - this->GetPosition().x > target_distance || (*it)->GetPosition().y - this->GetPosition().y > target_distance)
			continue;
		double d = glm::distance((*it)->GetPosition(), this->GetPosition());
		if (d == 0 || d > target_distance)
			continue;
		float l = 0.5f / d;
		if (l < 0.0f)
			continue;
		(*it)->AddVelocity(9.8f * (float)deltaTime * l * glm::normalize(glm::vec2((*it)->GetPosition() - this->GetPosition())));
		//this->force += -((*it)->GetCenter() - this->GetCenter());
	}
}
