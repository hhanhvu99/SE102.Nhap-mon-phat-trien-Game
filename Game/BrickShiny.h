#pragma once

#include "GameObject.h"


class BrickShiny : public GameObject
{
	int hp;
	float oldX, oldY;

	bool moving;
	DWORD startMoving;

public:
	BrickShiny(float x, float y, LPSPRITE sprite = NULL);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state); 

	~BrickShiny();
};
