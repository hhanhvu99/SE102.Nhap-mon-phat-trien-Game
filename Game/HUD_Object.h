#pragma once

#include "GameObject.h"

class HUD_Object : public GameObject
{
public:
	HUD_Object(LPSPRITE sprite);

	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }
	LPSPRITE GetSprite() { return this->sprite; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
};

