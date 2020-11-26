#pragma once

#include "GameObject.h"
#include "TestScene.h"

//Basic info
#define ENEMY_GROOMBA_WIDTH				14
#define ENEMY_GROOMBA_HEIGHT			16

#define ENEMY_GROMMBA_GRAVITY			0.0007f
#define ENEMY_GROOMBA_MOVE_SPEED_X		0.02f
#define ENEMY_GROOMBA_DEFLECT_Y			0.1f
#define ENEMY_GROOMBA_JUMP_SHORT		0.1f
#define ENEMY_GROOMBA_JUMP_LONG			0.2f

#define ENEMY_GROOMBA_TIME_LEFT			2000
#define ENEMY_GROOMBA_TIME_JUMP			1500

#define ENEMY_GROOMBA_MAX_JUMP			4

//Offset
#define ENEMY_GROOMBA_BBOX_OFFSET_X		1.0f
#define ENEMY_GROOMBA_DRAW_OFFSET_X		0.0f
#define ENEMY_GROOMBA_DRAW_OFFSET_Y		1.0f

//State
#define ENEMY_STATE_IDLE				0
#define ENEMY_STATE_MOVING				1
#define ENEMY_STATE_STOMP				2
#define ENEMY_STATE_HIT					3

class Groomba : public GameObject
{
	int mobType;
	int numberOfJump;
	int ani_walk_speed;
	int countOffGround;

	float nx, ny;
	float pointX, pointY;
	float offsetX=0, offsetY=0;
	float camPosX, camPosY;

	bool touchGround;
	bool hasWing;
	bool allowJump;
	bool hitByStomp;
	bool hitByBullet;
	bool pause;

	DWORD timeLeft;
	DWORD timeJump;

public:
	Groomba(int placeX, int placeY, int mobType, bool hasWing);

	void Add();
	void Destroy();
	void Remove();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
};

