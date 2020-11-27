#pragma once

#include "GameObject.h"
#include "TestScene.h"

//Basic info
#define ITEM_MOVE_SPEED_X			0.04f
#define ITEM_MOVE_SPEED_Y			0.02f
#define ITEM_JUMP_SPEED				0.2f
#define ITEM_GRAVITY				0.0005f

#define ITEM_LEAF_SPEED_X			0.0002f
#define ITEM_LEAF_SPEED_Y			0.00007f
#define ITEM_FRICTION				0.001f
#define ITEM_LEAF_BOUNDARY			0.1f

#define ITEM_SAFE_DELETE_RANGE		300.0f


class Item : public GameObject
{
	float boundaryUp;
	float camPosX, camPosY;
	int itemType;

	bool showUp;
	bool stop;
	bool leafBreak;
	bool leafBreak_Y;

public:
	Item(float x, float y, int itemType);

	void Add();
	void Destroy();
	int getItemType() { return this->itemType; }
	bool isShowUp() { return this->showUp; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);

	~Item();
};
