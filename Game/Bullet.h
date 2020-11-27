#pragma once
#include "GameObject.h"

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

