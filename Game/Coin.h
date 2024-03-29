#pragma once

#include "Item.h"
#include "TestScene.h"

class Coin : public Item
{
	float boundaryUp;
	bool outSide;
	LPGAMEOBJECT master;

public:
	Coin(float x, float y, int itemType, bool outSide);
	void SetMaster(LPGAMEOBJECT master) { this->master = master; }

	virtual void Add();
	virtual void Destroy();
	virtual void DestroyTouch();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);

	~Coin();
};

