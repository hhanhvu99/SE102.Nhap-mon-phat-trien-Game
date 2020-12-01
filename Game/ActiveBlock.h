#pragma once

#include "GameObject.h"

class ActiveBlock : public GameObject
{
protected:
	int hp;
	LPGAMEOBJECT item;

public:
	ActiveBlock() {};
	ActiveBlock(float x, float y, LPSPRITE sprite = NULL);

	void SetItem(LPGAMEOBJECT item) { this->item = item; }
	LPGAMEOBJECT GetItem() { return this->item; }
	bool hasItem() { return item; }

	~ActiveBlock();
};

