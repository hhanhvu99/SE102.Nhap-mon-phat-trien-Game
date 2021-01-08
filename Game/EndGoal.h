#pragma once

#include "TestScene.h"
#include "GameObject.h"

#define END_GOAL_ITEM_MOVE_SPEED		0.2f
#define END_GOAL_TIME					200	

class EndGoal : public GameObject
{
	int currentItem;
	DWORD timeStart = 0;

public:
	EndGoal(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void Destroy();
};

