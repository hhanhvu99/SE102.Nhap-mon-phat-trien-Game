#pragma once

#include "GameObject.h"
#include "TestScene.h"

//Basic Info
#define ENEMY_TROOP_MOVE_SPEED		0.015f
#define ENEMY_TROOP_BOUNDARY		8.0f

class EnemyTroop : public GameObject
{
	bool isMoving = false;
	float oldX, oldY;
	float newX, newY;
	float vectorX, vectorY, magnitude;

	float oldDist;

public:
	EnemyTroop(int indexX, int indexY, int mobType);

	void Add();
	void Destroy();
	void Remove();

	void MoveTo(float x, float y);
	bool IsMoving() { return this->isMoving; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};

