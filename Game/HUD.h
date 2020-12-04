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

#define HUD_PLAYER_LIVE_X			36
#define HUD_PLAYER_LIVE_Y			17

#define HUD_WORLD_X					44
#define HUD_WORLD_Y					9

#define HUD_SPEED_O_METER_X			59
#define HUD_SPEED_O_METER_Y			9

#define HUD_POINT_X					59
#define HUD_POINT_Y					17

#define HUD_MONEY_X					139
#define HUD_MONEY_Y					9

#define HUD_TIME_X					131
#define HUD_TIME_Y					17

#define HUD_CARD_ONE_X				168
#define HUD_CARD_ONE_Y				3

#define HUD_CARD_TWO_X				192
#define HUD_CARD_TWO_Y				3

#define HUD_CARD_THREE_X			216
#define HUD_CARD_THREE_Y			3

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


