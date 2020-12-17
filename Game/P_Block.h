#pragma once

#include "SceneManager.h"
#include "TestScene.h"
#include "GameObject.h"
#include "Coin.h"

class P_Block : public GameObject
{
	LPSPRITE p_Block;
	LPSPRITE stomp_Block;
	vector<LPGAMEOBJECT> listOfObject;

public:
	P_Block(float x, float y);

	void AddObject(LPGAMEOBJECT object);
	void ChangeToCoin();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
};

