#pragma once

#include "Item.h"
#include "TestScene.h"

class Flower : public Item
{
	float boundaryUp;
	bool showUp;

public:
	Flower(float x, float y, int itemType);

	virtual void Add();
	virtual void Destroy();
	virtual void DestroyTouch();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);

	~Flower();
};

