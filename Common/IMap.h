#pragma once
#include "Library.h"

class IMap
{
public:
	virtual int GetSpawnX() = 0;
	virtual int GetSpawnY() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetBlock(int i, int j) = 0;
};