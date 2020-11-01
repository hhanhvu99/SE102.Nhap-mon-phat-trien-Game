#pragma once
#include "Bullet.h"
#include "TestScene.h"

//Basic info
#define BULLET_HAMMER_WIDTH				8
#define BULLET_HAMMER_HEIGHT			16

#define BULLET_HAMMER_SPEED_X			0.05f
#define BULLET_HAMMER_GRAVITY			0.0005f
#define BULLET_HAMMER_SPEED_THROW		0.2f
//Offset
#define BULLET_HAMMER_DRAW_OFFSET_X		0.0f
#define BULLET_HAMMER_DRAW_OFFSET_Y		0.0f
#define BULLET_SAFE_DELETE_RANGE		300.0f

class Hammer : public Bullet
{
public:
	Hammer(float x, float y, int direction);

	virtual void Add();
	virtual void Destroy();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);

	~Hammer();
};