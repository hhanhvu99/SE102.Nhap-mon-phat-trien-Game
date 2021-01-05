#pragma once

#include "GameObject.h"
#include "Boomerang.h"
#include "TestScene.h"

//Basic info
#define ENEMY_BRO_WIDTH					14
#define ENEMY_BRO_HEIGHT				22

#define ENEMY_BRO_GRAVITY				0.0007f
#define ENEMY_BRO_MOVE_SPEED_X			0.02f
#define ENEMY_BRO_DEFLECT				0.1f
#define ENEMY_BRO_JUMP					0.1f

#define ENEMY_BRO_TIME_SWITCH			2000
#define ENEMY_BRO_TIME_THROW_READY		2000
#define ENEMY_BRO_TIME_THROW_WAIT		8000
#define ENEMY_BRO_TIME_JUMP				5000

#define ENEMY_BRO_THROW_RANGE			96.0f

//State
#define ENEMY_STATE_IDLE				0
#define ENEMY_STATE_MOVING				1
#define ENEMY_STATE_STOMP				2
#define ENEMY_STATE_HIT					3

class BoomerangBro : public GameObject
{
	int mobType;
	LPGAMEOBJECT mario;

	float camPosX, camPosY;

	int facing = -1;
	bool steady;

	DWORD timeLeft;
	DWORD timeJump;
	DWORD timeThrow;

	//Pausing outside camera
	bool firstRun = true;
	int lastState;
	DWORD timeLeft_dt;
	DWORD timeJump_dt;
	DWORD timeThrow_dt;
public:
	BoomerangBro(int placeX, int placeY, int mobType);

	void Add();
	void Destroy();
	void Remove();
	void AddMario(LPGAMEOBJECT mario) { this->mario = mario; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
};

