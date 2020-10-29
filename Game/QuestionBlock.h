#pragma once

#include "GameObject.h"

class QuestionBlock : public GameObject
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
	virtual void SetState(int state) { this->state = state; }

	~QuestionBlock();
};
