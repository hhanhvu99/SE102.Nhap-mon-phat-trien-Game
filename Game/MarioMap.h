#pragma once

#include "GameObject.h"

//Basic Info
#define MARIO_MAP_MOVE_SPEED			0.1f

//State
#define MARIO_MAP_STATE_IDLE			0
#define MARIO_MAP_STATE_ROLLING			100

class MarioMap : public GameObject
{
	GLOBAL global;

	bool isMoving = false;
	bool isRolling = false;
	float newX, newY;
	float vectorX, vectorY, magnitude;

	float oldDist;

public:
	MarioMap(float x = 0.0f, float y = 0.0f);

	void MoveTo(float x, float y);
	bool IsMoving() { return this->isMoving; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void Destroy() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void SetState(int state);
};

