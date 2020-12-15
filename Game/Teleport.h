#pragma once

#include "TestScene.h"
#include "Mario.h"
#include "MarioMap.h"

class Teleport : public GameObject
{
	LPGAMEOBJECT mario;
	GLOBAL global;

	int currentScene;
	int targetScene;

	float currentX, currentY;
	float targetX, targetY;
	float marioX, marioY;

	bool allowSwitch;

public:
	Teleport(int targetScene);
	Teleport(int currentScene, float x, float y, int targetScene);
	Teleport(int currentScene, float x, float y, int targetScene, float tx, float ty, int direction);

	void Add();
	void Destroy();

	bool IsAllowSwitch() { return allowSwitch; }
	int GetTargetID() { return targetScene; }
	void GetTargetPos(float& x, float& y) { x = targetX; y = targetY; }
	void GetCurrentPos(float& x, float& y) { x = currentX; y = currentY; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
	virtual void SetState(int state) { this->state = state; }

};

