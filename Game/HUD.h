#pragma once

#include "GameObject.h"
#include "TestScene.h"
#include "HUD_Object.h"

//Basic Info
#define HUD_BUBBLE_JUMP			0.1f
#define HUD_BUBBLE_GRAVITY		0.0005f

//Offset
#define HUD_ARROW_SPACING		8

//Position
#define HUD_PLAYER_ICON_X			11
#define HUD_PLAYER_ICON_Y			17

#define HUD_PLAYER_LIVE_X			35
#define HUD_PLAYER_LIVE_Y			17

#define HUD_WORLD_X					43
#define HUD_WORLD_Y					9

#define HUD_SPEED_O_METER_X			58
#define HUD_SPEED_O_METER_Y			9

//Time
#define HUD_BUTTON_FLASH_TIME		150
#define HUD_BUTTON_FLASH_TIME_2		300

class HUD : public GameObject
{
	bool isBubble;
	int number;
	int oldNumber;

	GameEngine* instance;
	GLOBAL global;

	HUD_Object* object;
	vector<HUD_Object*> spriteHolder;

	DWORD buttonFlashing = 0;

public:
	HUD(eType type);
	HUD(float x, float y, bool isBubble, int number, eType type);

	void Add();
	void Destroy();
	void Setup();
	vector<int> numberToList(int number, int n);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	
	~HUD();
};


