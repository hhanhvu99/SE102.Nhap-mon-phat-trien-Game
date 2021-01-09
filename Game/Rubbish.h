#pragma once

#include "GameObject.h"
#include "TestScene.h"

#define RUBBISH_ROTATE_SPEED		2.0f
#define RUBBISH_SPEED_X				0.005f
#define RUBBISH_SPEED_Y				0.0008f
#define RUBBISH_DEFLECT_SPEED		0.2f

class Rubbish : public GameObject
{
	float currentDergee;
	float speedX, speedY;
	float camPosX, camPosY;

public:
	Rubbish(float x, float y, float speedX, float speedY, float deflect, int direction);

	void Add();
	void Destroy();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state) { this->state = state; }

	virtual ~Rubbish() {};
};

