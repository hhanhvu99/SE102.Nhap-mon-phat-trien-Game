#pragma once

#include "TestScene.h"
#include "BackGround.h"
#include "MarioMap.h"
#include "EnemyTroop.h"
#include "HUD.h"

//Time
#define MAP_POPUP			2000
#define MAP_BLACK_TITLE		10

//Position
#define MAP_POS_ARROW_1_X	100.0f
#define MAP_POS_ARROW_1_Y	72.0f
#define MAP_POS_ARROW_2_X	100.0f
#define MAP_POS_ARROW_2_Y	80.0f

struct Path {
	BackGround* currentPath;
	int type;
	int adjacent[8];
	bool isFinished = false;
};

typedef Path* LPPATH;

class WorldMap : public TestScene
{
	std::unordered_map<int, LPPATH> listOfPath;
	int mapTitle[500][500];
	
	int indexX, indexY;
	int direction;

	float startX, startY;
	LPPATH current;

	MarioMap* castMario;

	bool showPopup = false;
	bool startTime = true;
	DWORD timePass = 0;

	bool endScene = false;
	DWORD titleTime = 0;

	bool gameOver = false;
	bool firstOver = true;
	bool firstOption = true;

	//Temp
	BackGround* startScene;
	BackGround* world;
	BackGround* marioIcon;
	BackGround* numberOne;
	BackGround* numberTwo;

	BackGround* text;
	BackGround* arrow;

public:
	WorldMap(int id, LPCWSTR filePath);

	void Reset();
	void Restart();
	bool IsEndScene() { return endScene; }

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void SetState(int state);

	//CMario* GetPlayer() { return player; }
};

