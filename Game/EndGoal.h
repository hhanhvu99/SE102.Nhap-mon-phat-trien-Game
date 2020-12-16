#pragma once

#include "GameObject.h"

#define END_GOAL_ITEM_MOVE_SPEED		0.2f

class EndGoal : public GameObject
{
	int currentItem;

public:
	EndGoal(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
};

