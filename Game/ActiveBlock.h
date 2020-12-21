#pragma once

#include "GameObject.h"
#include "Item.h"

class ActiveBlock : public GameObject
{
protected:
	int hp;
	Item* item;

public:
	ActiveBlock() {};
	ActiveBlock(float x, float y, LPSPRITE sprite = NULL);

	void SetItem(Item* item) { this->item = item; }
	Item* GetItem() { return this->item; }
	bool hasItem() { return item; }

	~ActiveBlock();
};

