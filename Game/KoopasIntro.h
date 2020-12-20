#pragma once

#include "GameObject.h"
#include "TestScene.h"
#include "PlayerIntro.h"

//Basic info
#define ENEMY_MENU_KOOPAS_GRAVITY				0.0007f
#define ENEMY_MENU_KOOPAS_MOVE_SPEED_X			0.03f
#define ENEMY_MENU_KOOPAS_MOVE_SPEED_FAST		0.05f
#define ENEMY_MENU_KOOPAS_ROLL_SPEED_DEFLECT	0.07f
#define ENEMY_MENU_KOOPAS_ROLL_SPEED_X			0.22f
#define ENEMY_MENU_KOOPAS_ROLL_SPEED_CUSTOM		0.18f
#define ENEMY_MENU_KOOPAS_ROLL_SPEED_SLOW		0.15f
#define ENEMY_MENU_KOOPAS_DEFLECT_Y				0.003f
#define ENEMY_MENU_KOOPAS_DEFLECT				0.2f


//State
#define ENEMY_STATE_RELEASE				8
#define ENEMY_STATE_ROLL_SPECIAL		9
#define ENEMY_STATE_BEING_GRAB			10

class Mario;
class KoopasIntro : public GameObject
{
	PlayerIntro* mario;

	int mobType;

	float nx, ny;
	float min_tx, min_ty;
	float pointX, pointY;
	float offsetX = 0, offsetY = 0;
	float camPosX, camPosY;
	float rollSpeed;

	bool immobilize;
	bool rolling;
	bool beingGrab;
	bool upSideDown;

	bool special;

public:
	KoopasIntro(int placeX, int placeY, int mobType, bool inShell);

	void Add();
	void Destroy();
	void Remove();
	void SetRollSpeed(float speed) { this->rollSpeed = speed; }
	void SetGrapper(PlayerIntro* player);
	void IsSpecial() { this->special = true; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
};
