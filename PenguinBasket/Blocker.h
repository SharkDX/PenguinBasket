#pragma once
class Blocker
{
public:
	int X;
	int Y;
	int sizeX;
	int sizeY;
	int originX;
	int originY;

	Blocker(int x, int y)
	{
		X = x;
		Y = y;
		sizeX = 3;
		sizeY = 6;
		originY = 5;
		originX = 1;
	}
};

