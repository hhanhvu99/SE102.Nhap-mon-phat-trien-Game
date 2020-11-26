#pragma once
#include "Bullet.h"
#include "TestScene.h"

//Basic info
#define BULLET_FIREBALL_WIDTH			8
#define BULLET_FIREBALL_HEIGHT			8

#define BULLET_FIREBALL_SPEED_X			0.15f
#define BULLET_FIREBALL_GRAVITY			0.001f
#define BULLET_FIREBALL_DEFLECT_SPEED	0.2f

//Offset
#define BULLET_FIREBALL_DRAW_OFFSET_X	0.0f
#define BULLET_FIREBALL_DRAW_OFFSET_Y	-4.0f
#define BULLET_EFFECT_DRAW_OFFSET_X		-4.0f
#define BULLET_EFFECT_DRAW_OFFSET_Y		-4.0f
#define BULLET_SAFE_DELETE_RANGE		300.0f

//Time left
#define BULLET_FIREBALL_TIME_LEFT		200
class Fireball : public Bullet
{
	float unitVectorX, unitVectorY;

public:
	Fireball(float x, float y, int direction);
	Fireball(float x, float y, int direction, float unitVectorX, float unitVectorY);

	void SetBulletType(int type) { this->bulletType = type; }
	void Add();
	void Destroy();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);

	~Fireball();
};