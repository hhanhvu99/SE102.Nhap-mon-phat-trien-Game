#pragma once

#include "TestScene.h"
#include "GroupObject.h"
#include "BackGround.h"
#include "PlayerIntro.h"
#include "KoopasIntro.h"
#include "Groomba.h"
#include "Mushroom.h"
#include "SuperLeaf.h"
#include "SuperStar.h"

//Basic Info
#define MENU_RIBBON_MOVE_Y		0.1f
#define MENU_RIBBON_LIMIT		200.0f

#define MENU_TITLE_MOVE_Y		0.3f
#define MENU_TITLE_VIBRATION	2.0f
#define MENU_TITLE_MAX_TIME		1000
#define MENU_TITLE_Y			16.0f
#define MENU_NUMBER_Y			96.0f

//Time
#define MENU_TIME_STAGE_ONE		1000
#define MENU_TIME_STAGE_TWO		1050
#define MENU_TIME_STAGE_THREE	1100
#define MENU_TIME_STAGE_FOUR	1150
#define MENU_TIME_TITLE_WAIT	700

#define MENU_TIME_ENTITY_ONE	500
#define MENU_TIME_ENTITY_TWO	1500
#define MENU_TIME_ENTITY_THREE	2000

#define MENU_MARIO_TITLE_1		200
#define MENU_MARIO_TITLE_2		400
#define MENU_MARIO_TITLE_3		800
#define MENU_MARIO_TITLE_4		1600

#define MENU_LUIGI_POSITION_X	304.0f
#define MENU_LUIGI_POSITION_Y	144.0f

#define MENU_MARIO_SECOND_1		2200
#define MENU_MARIO_SECOND_2		2500
#define MENU_MARIO_SECOND_3		3500

class Intro : public TestScene
{
	PlayerIntro* mario;
	PlayerIntro* luigi;
	BackGround* ribbon;
	BackGround* ribbon_top;
	BackGround* title;
	BackGround* tree1;
	BackGround* tree2;
	BackGround* number;

	Groomba* groomba;
	KoopasIntro* turtleShell;

	bool isFirst = true;

	bool moveRibbon = true;
	bool setSpeed = true;

	bool moveTitle = false;
	bool shaking = false;
	bool startShaking = true;
	float shakeX, shakeY;
	float shakeXNum, shakeYNum;
	int sign = 1;
	DWORD shakingTime = 0;
	DWORD waittingTime = 0;

	bool allowTranform = false;
	bool startTranform = true;
	DWORD tranformTime = 0;

	bool allowCreate = true;
	bool allowEntity = false;
	bool startEntity = true;
	DWORD timeEntity = 0;

	bool stageTwoFirst = true;
	bool allowCreateTurtleShell = true;

	bool allowPartOne = true;

	bool hitByShell = true;
	DWORD timeHitShell = 0;

	bool inRaccoonFirst = true;
	DWORD timeRaccoon = 0;

	bool allowPartTwo = false;
	bool turtleShellFirst = true;

public:
	Intro(int id, LPCWSTR filePath);

	bool OutSideCam(LPGAMEOBJECT object);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//CMario* GetPlayer() { return player; }
};

