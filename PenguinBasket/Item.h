#pragma once
#include <glm\glm.hpp>
#include <memory>

class Item
{
protected:
	Item();
public:
	static Item* pickaxe;
	static Item* ingot;

	int Id = 0;
	int stack = 1;
	int textureIndex = 0;
	bool block = false;
	int maxStack = 99;
	bool continuous = false;
	char useTime = 30;
	bool consumable = true;
	char* name = "";

	static Item* items[];
	static void InitItems();

	~Item();
	virtual bool Use(double mouseX, double mouseY);
	virtual bool Special(double mouseX, double mouseY);
	virtual Item* clone() const;
};