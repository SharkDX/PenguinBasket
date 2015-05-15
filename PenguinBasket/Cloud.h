#pragma once
#include <glm\glm.hpp>

class Cloud
{
public:
	glm::vec2 pos = glm::vec2();
	glm::vec2 size = glm::vec2(4.0f, 1.0f);
	char type = 0;

	Cloud()
	{
	}

	static bool comp(Cloud* c1, Cloud* c2) {
		return (c1->size.length() < c2->size.length()); 
	}
};

