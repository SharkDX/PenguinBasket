#include "Inventory.h"


Inventory::Inventory(const int size)
{
	this->Size = size;
	items = new std::shared_ptr<Item>[size];
	for (int i = 0; i < size; i++)
		items[i] = nullptr;
}


Inventory::~Inventory()
{
	delete items;
}

std::shared_ptr<Item> Inventory::GetItem(int i)
{
	if (i < 0 || i >= this->Size)
		return nullptr;
	return items[i];
}

int Inventory::FindSlot(int id)
{
	for (int i = 0; i < Size; i++)
	{
		std::shared_ptr<Item> it = GetItem(i);
		if (it != nullptr && it->Id == id && it->stack < it->maxStack)
			return i;
	}
	return -1;
}

int Inventory::FindEmptySlot()
{
	for (int i = 0; i < Size; i++)
	{
		if (GetItem(i) == nullptr)
			return i;
	}
	return -1;
}

bool Inventory::IsFull()
{
	return FindEmptySlot() == -1;
}

bool Inventory::AddItem(Item* item, int stack)
{
	int slot = FindSlot(item->Id);
	if (slot == -1)
	{
		slot = FindEmptySlot();
		if (slot == -1)
			return false;
		std::shared_ptr<Item> inventoryItem = std::shared_ptr<Item>(item->clone());
		inventoryItem->stack = stack;
		items[slot] = inventoryItem;
	}
	else
	{
		items[slot]->stack += item->stack;
	}
	if (items[slot]->stack > items[slot]->maxStack)
	{
		int remain = items[slot]->stack - items[slot]->maxStack;
		items[slot]->stack -= remain;
		item->stack = remain;
		return AddItem(item);
	}
	return true;
}

bool Inventory::UseItem(int slot, double mouseX, double mouseY)
{
	std::shared_ptr<Item> item = GetItem(slot);
	if (item == nullptr)
		return false;
	if (item->Use(mouseX, mouseY))
	{
		if (item->consumable)
			--item->stack;
	}
	if (item->stack <= 0)
	{
		items[slot] = nullptr;
	}
	return true;
}
