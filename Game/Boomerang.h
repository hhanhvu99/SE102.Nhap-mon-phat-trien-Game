#pragma once
#include "Bullet.h"
#include "TestScene.h"

//Basic info
#define BULLET_BOOMERANG_WIDTH			12
#define BULLET_BOOMERANG_HEIGHT			12

#define BULLET_BOOMERANG_START_VX		0.2f
#define BULLET_BOOMERANG_START_VY		0.1f
#define BULLET_BOOMERANG_REFLECT_VX		0.0002f
#define BULLET_BOOMERANG_REFLECT_VY		0.00018f

#define BULLET_BOOMERANG_OFFSET_Y		4.0f

class Boomerang : public Bullet
{
	float throwerX, throwerY;
	LPGAMEOBJECT thrower;

public:
	Boomerang(float x, float y, int direction, LPGAMEOBJECT thrower);

	void Add();
	void Destroy();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);

	~Boomerang();
};