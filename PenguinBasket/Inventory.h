#pragma once
#include "Item.h"
#include "ItemBlock.h"

class Inventory
{
public:
	int Size;

	Inventory(const int size);
	~Inventory();

	std::shared_ptr<Item> GetItem(int i);
	int FindSlot(int id);
	int FindEmptySlot();
	bool IsFull();
	bool AddItem(Item* item, int stack = 1);
	bool UseItem(int slot, double mouseX, double mouseY);
private:
	std::shared_ptr<Item>* items;
};

