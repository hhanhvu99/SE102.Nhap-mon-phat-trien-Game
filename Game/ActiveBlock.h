#pragma once

#include "GameObject.h"
#include "Item.h"

class ActiveBlock : public GameObject
{
protected:
	int hp;
	int option;
	Item* item;

public:
	ActiveBlock() {};
	ActiveBlock(float x, float y, LPSPRITE sprite = NULL);

	void SetOption(int option) { this->option = option; }
	void SetHP(int hp) { this->hp = hp; }
	void SetItem(Item* item) { this->item = item; }
	Item* GetItem() { return this->item; }
	bool hasItem() { return item; }

	~ActiveBlock();
};

