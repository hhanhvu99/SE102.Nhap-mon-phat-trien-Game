#pragma once

#include "GameObject.h"
#include "Sprite.h"

#define BACKGROUND_BLOCK_WIDTH	16
#define BACKGROUND_BLOCK_HEIGHT	16

class BackGround : public GameObject
{
	bool allowDraw;
	D3DCOLOR color;

public:
	BackGround(float x, float y, LPSPRITE sprite);

	void SetAllowDraw() { this->allowDraw = true; }
	void SetDisableDraw() { this->allowDraw = false; }
	void SetDrawColor(D3DCOLOR color) { this->color = color; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};