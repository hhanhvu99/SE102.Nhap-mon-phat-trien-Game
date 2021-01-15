#pragma once

#include "GameObject.h"
#include "TestScene.h"
#include "HUD_Object.h"

//Basic Info
#define HUD_BUBBLE_JUMP				0.07f
#define HUD_BUBBLE_GRAVITY			0.0001f

//Offset
#define HUD_ARROW_SPACING			8

//Position
#define HUD_PLAYER_ICON_X			19
#define HUD_PLAYER_ICON_Y			17

#define HUD_PLAYER_LIVE_X			44
#define HUD_PLAYER_LIVE_Y			17

#define HUD_WORLD_X					52
#define HUD_WORLD_Y					9

#define HUD_SPEED_O_METER_X			67
#define HUD_SPEED_O_METER_Y			9

#define HUD_POINT_X					67
#define HUD_POINT_Y					17

#define HUD_MONEY_X					147
#define HUD_MONEY_Y					9

#define HUD_TIME_X					139
#define HUD_TIME_Y					17

#define HUD_CARD_ONE_X				175
#define HUD_CARD_ONE_Y				3

#define HUD_CARD_TWO_X				199
#define HUD_CARD_TWO_Y				3

#define HUD_CARD_THREE_X			223
#define HUD_CARD_THREE_Y			3

#define HUD_TEXT_1_X				75
#define HUD_TEXT_1_Y				-160

#define HUD_TEXT_2_X				59
#define HUD_TEXT_2_Y				-136

#define HUD_ITEM_ICON_X				183
#define HUD_ITEM_ICON_Y				-142

//Time
#define HUD_BUTTON_FLASH_TIME		149
#define HUD_BUTTON_FLASH_TIME_2		300
#define HUD_SPLASH_EFFECT_TIME		100

class HUD : public GameObject
{
	bool isBubble;
	int number;
	int oldNumber;

	GameEngine* instance;
	GLOBAL global;

	HUD_Object* object;
	LPANIMATION ani;
	vector<HUD_Object*> spriteHolder;

	DWORD splashTime = 0;
	DWORD buttonFlashing = 0;

public:
	HUD(eType type);
	HUD(float x, float y, int number);
	HUD(float x, float y, LPANIMATION ani);
	HUD(float x, float y, LPSPRITE sprite);

	void Add();
	void Destroy();
	void Setup();
	static string numberToString(int number, unsigned int n);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	
	~HUD();
};


