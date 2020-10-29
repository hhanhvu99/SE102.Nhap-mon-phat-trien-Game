#pragma once

#include "GameObject.h"

class ActiveBlock : public GameObject
{
	LPANIMATION animationSet;
	int hp;

public:
	ActiveBlock(float x, float y, int id, LPSPRITE sprite = NULL);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() {};
	virtual void SetState(int state) { this->state = state; }

	~ActiveBlock();
};

