#pragma once

#include "GameObject.h"
#include "TestScene.h"
#include "Mario.h"

//Basic info
#define ENEMY_KOOPAS_WIDTH				14
#define ENEMY_KOOPAS_HEIGHT				16

#define ENEMY_KOOPAS_GRAVITY			0.0005f
#define ENEMY_KOOPAS_MOVE_SPEED_X		0.02f
#define ENEMY_KOOPAS_ROLL_SPEED_X		0.17f
#define ENEMY_KOOPAS_DEFLECT_Y			0.1f
#define ENEMY_KOOPAS_JUMP_SPEED			0.15f

#define ENEMY_KOOPAS_TIME_LEFT			8000
#define ENEMY_KOOPAS_TIME_COMEBACK		7000
#define ENEMY_KOOPAS_TIME_SHAKE			6000
#define ENEMY_KOOPAS_ANI_SHAKE_TIME		10

#define ENEMY_KOOPAS_ANI_SHAKE_X		1.0f

//Offset
#define ENEMY_KOOPAS_BBOX_OFFSET_X		1.0f
#define ENEMY_KOOPAS_DRAW_OFFSET_X		0.0f
#define ENEMY_KOOPAS_DRAW_OFFSET_Y		1.0f

//State
#define ENEMY_STATE_IDLE				0
#define ENEMY_STATE_MOVING				1
#define ENEMY_STATE_STOMP				2
#define ENEMY_STATE_HIT					3
#define ENEMY_STATE_ROLLING				4
#define ENEMY_STATE_KICK				5

class Mario;
class Koopas : public GameObject
{
	Mario* mario = NULL;

	int mobType;

	float nx, ny;
	float min_tx, min_ty;
	float pointX, pointY;
	float offsetX = 0, offsetY = 0;
	float camPosX, camPosY;
	float shakeX;

	bool hasWing;
	bool immobilize;
	bool rolling;
	bool comeBack;
	bool beingGrab;

	bool startShaking;
	bool shaking;
	DWORD shakeTime;

	DWORD timeLeft;

public:
	Koopas(int placeX, int placeY, int mobType, bool hasWing);

	void Add();
	void Destroy();
	void Remove();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
};
