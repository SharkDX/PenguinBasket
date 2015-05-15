#pragma once
#include "glm\glm.hpp"

class Shape {
public:
	Shape(double x, double y) 
	{ 
		SetPos(x, y); 
	};

	double GetX() { return x; };
	double GetY() { return y; };

	void SetPos(double x, double y) { this->x = x; this->y = y; };

	
protected:
	double x;
	double y;
};