#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
#pragma message ("Bullet added")
protected:
	float nx, ny;
	float offsetX, offsetY;
	float camPosX, camPosY;
	int bulletType;
	DWORD timeLeft;
	LPGAMEOBJECT mario;

	bool pause = false;

public:
	virtual void Add() = 0;
	virtual void Destroy() = 0;

};

