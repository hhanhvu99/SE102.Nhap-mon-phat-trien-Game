#pragma once

#include "TestScene.h"
#include "GameObject.h"
#include "Sprite.h"

#define BLOCK_WIDTH		16
#define BLOCK_HEIGHT	16

class Block : public GameObject
{
public:
	Block(float x, float y, LPSPRITE sprite);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL) {};
	virtual void Render();
	virtual void Destroy();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};