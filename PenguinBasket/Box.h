#pragma once
#include "Shape.h"

class Box : public Shape {
public:
	Box(double x, double y, double w, double h) 
		: Shape(x, y)
	{
		SetSize(w, h);
	};

	double GetW() { return w; };
	double GetH() { return h; };
	double GetCenterX() { return x + w / 2; };
	double GetCenterY() { return y + h / 2; };

	void SetSize(double w, double h) { this->w = w; this->h = h; };
	bool Contains(double x, double y)
	{
		if (GetX() < x && GetY() < y && GetX() + GetW() > x && GetY() + GetH())
			return true;
		return false;
	}

	virtual bool Intersects(Box* box, float &depthX, float &depthY)
	{
		double halfWidthA = GetW() / 2;
		double halfHeightA = GetH() / 2;
		double halfWidthB = box->GetW() / 2;
		double halfHeightB = box->GetH() / 2;

		double XcenterA = GetX() + halfWidthA;
		double YcenterA = GetY() + halfHeightA;
		double XcenterB = box->GetX() + halfWidthB;
		double YcenterB = box->GetY() + halfHeightB;

		double distanceX = XcenterA - XcenterB;
		double distanceY = YcenterA - YcenterB;
		double minDistanceX = halfWidthA + halfWidthB;
		double minDistanceY = halfHeightA + halfHeightB;

		if (abs(distanceX) > minDistanceX || abs(distanceY) > minDistanceY)
		{
			depthX = depthY = 0;
			return false;
		}
		depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
		depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
		return true;
	}
private:
	double w;
	double h;
};