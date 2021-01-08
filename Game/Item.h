#pragma once

#include "GameObject.h"

//Basic info
#define ITEM_MOVE_SPEED_X			0.04f
#define ITEM_MOVE_SPEED_Y			0.02f
#define ITEM_JUMP_SPEED				0.2f
#define ITEM_GRAVITY				0.0005f

#define ITEM_LEAF_SPEED_X			0.00025f
#define ITEM_LEAF_SPEED_Y			0.00022f
#define ITEM_FRICTION				0.0008f
#define ITEM_FRICTION_UP			0.00023f
#define ITEM_LEAF_BOUNDARY			0.1f

#define ITEM_COIN_GRAVITY			0.001f
#define ITEM_COIN_JUMP_SPEED		0.3f

#define ITEM_SAFE_DELETE_RANGE		600.0f


class Item : public GameObject
{
protected:
	float camPosX, camPosY;
	int itemType;

public:
	Item(float x, float y, int itemType);

	virtual void Add() = 0;
	virtual void Destroy() = 0;
	virtual void DestroyTouch() = 0;
	virtual void Replace() {};
	int getItemType() { return this->itemType; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};
