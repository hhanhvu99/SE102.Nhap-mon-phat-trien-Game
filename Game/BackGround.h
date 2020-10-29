#pragma once

#include "GameObject.h"
#include "Sprite.h"

#define BACKGROUND_BLOCK_WIDTH	16
#define BACKGROUND_BLOCK_HEIGHT	16

class BackGround : public GameObject
{
public:
	BackGround(float x, float y, LPSPRITE sprite);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL) {};
	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};