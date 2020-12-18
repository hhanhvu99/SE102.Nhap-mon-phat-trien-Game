#pragma once
#include "GameObject.h"

#define BULLET_SAFE_DELETE_RANGE		50.0f

class Bullet : public GameObject
{
protected:
	float nx, ny;
	float offsetX, offsetY;
	float camPosX, camPosY;
	int bulletType;

	DWORD timeLeft;
	LPGAMEOBJECT mario;

public:
	virtual void Add() = 0;
	virtual void Destroy() = 0;

};

