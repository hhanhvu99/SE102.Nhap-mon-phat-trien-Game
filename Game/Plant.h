#pragma once

#include "GameObject.h"
#include "TestScene.h"
#include "Fireball.h"

//Basic info
#define ENEMY_PLANT_WIDTH				16
#define ENEMY_PLANT_HEIGHT_SHORT		24
#define ENEMY_PLANT_HEIGHT_LONG			32

#define ENEMY_PLANT_MOVE_SPEED			0.05f

#define ENEMY_PLANT_CYCLE_TIME			3000
#define ENEMY_PLANT_SHOOT_TIME			1500

//Offset
#define ENEMY_PLANT_OFFSET_X			8.0f
#define ENEMY_PLANT_BBOX_OFFSET_X		1.0f
#define ENEMY_PLANT_DRAW_OFFSET_X		0.0f
#define ENEMY_PLANT_DRAW_OFFSET_Y		1.0f

//State
#define ENEMY_STATE_IDLE				0
#define ENEMY_STATE_MOVING				1
#define ENEMY_STATE_HIT					3
#define ENEMY_STATE_SHOOT				6

class Plant : public GameObject
{
	int mobType;
	LPSCENE currentScene;

	float pointX, pointY;
	int direct;
	float vectorX;
	float vectorY;
	float magnitude;

	float marioPos_x, marioPos_y;
	float boundaryY_UP, boundaryY_DOWN;
	float nx, ny;
	float offsetX = 0, offsetY = 0;
	float camPosX, camPosY;

	bool pause;

	bool showFace;
	bool moving;

	DWORD timeShoot;
	DWORD timePass;

public:
	Plant(int placeX, int placeY, int mobType);

	void Add();
	void Destroy();
	void Remove();
	int GetDirection();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
};
