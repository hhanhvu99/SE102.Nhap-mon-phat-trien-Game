#pragma once

#include "TestScene.h"
#include "GroupObject.h"
#include "BackGround.h"

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

class Intro : public TestScene
{
	BackGround* ribbon;
	BackGround* ribbon_top;
	BackGround* title;
	BackGround* tree1;
	BackGround* tree2;
	BackGround* number;

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

	bool allowTranform = false;
	bool startTranform = true;
	DWORD tranformTime = 0;

public:
	Intro(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//CMario* GetPlayer() { return player; }
};

