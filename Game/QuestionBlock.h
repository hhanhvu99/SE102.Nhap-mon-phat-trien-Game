#pragma once

#include "ActiveBlock.h"
#include "Mushroom.h"
#include "Coin.h"

class QuestionBlock : public ActiveBlock
{
	bool hit;
	bool moving;
	float oldX, oldY;

	DWORD startMoving;

public:
	QuestionBlock(float x, float y, LPSPRITE sprite = NULL);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void Destroy();
	virtual void SetState(int state) { this->state = state; }

	~QuestionBlock() {};
};
