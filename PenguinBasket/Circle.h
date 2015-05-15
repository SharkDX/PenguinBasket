#pragma once
#include "Shape.h"
#include "Box.h"

class Circle : public Shape {
public:
	Circle(double x, double y, double r)
		: Shape(x, y)
	{
		SetRadius(r);
	};

	double GetR() { return r; };

	void SetRadius(double r) { this->r = r;};

	virtual bool Intersects(Circle* circle, float &depth)
	{
		double distance = glm::distance(glm::vec2(GetX(), GetY()), glm::vec2(circle->GetX(), circle->GetY()));
		depth = circle->GetR() + this->GetR() - distance;
		if (depth > 0)
			return true;
		depth = 0;
		return false;
	}

	virtual bool Intersects(Box* box, float &depthX, float &depthY)
	{
		double dX = abs(box->GetCenterX() - this->GetX());
		double dY = abs(box->GetCenterY() - this->GetY());

		if (dX < this->GetR() + box->GetW() / 2 && dY < this->GetR() + box->GetH() / 2)
			return true;

		return false;
	}
private:
	double r;
};