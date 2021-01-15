#pragma once

#include "Scene.h"
#include "Keyboard.h"

#include "Block.h"
#include "BackGround.h"
#include "GroupObject.h"

#include "BrickShiny.h"
#include "QuestionBlock.h"
#include "P_Block.h"

#include "Groomba.h"
#include "Koopas.h"
#include "Plant.h"
#include "BoomerangBro.h"

#include "SuperLeaf.h"
#include "SuperStar.h"
#include "Mushroom.h"
#include "Coin.h"
#include "Flower.h"

#include "EnemyTroop.h"
#include "Teleport.h"
#include "EndGoal.h"

#include "Grid.h"
#include "HUD.h"

#include "Global.h"


#define FLOAT_TEXT_HEIGHT		8.0f
#define SWITCH_TIME				200


using namespace std;
class TestScene: public Scene
{
protected:
	GLOBAL global;
	LPGAMEOBJECT mario;
	LPGAMEOBJECT p_Block_Temp;

	vector<LPGAMEOBJECT> gameObjects;
	vector<LPGAMEOBJECT> collideObjects;
	vector<LPGAMEOBJECT> deleteList;
	vector<LPGAMEOBJECT> addList;

	vector<LPGAMEOBJECT> teleport;
	LPGAMEOBJECT currentGate;

	vector<LPGAMEOBJECT> cells;

	int combo = 0;
	int soLanUpdate = 0;
	int currentWorld;
	bool allowResetStart = true;

	bool startTimerSwitch = true;
	DWORD timeSwitch = 0;

public:
	TestScene(int id, LPCWSTR filePath);

	void CheckCell();

	void Add(LPGAMEOBJECT gameObject);
	void Destroy(LPGAMEOBJECT gameObject);
	void Remove(LPGAMEOBJECT gameObject);
	void Add_Visual(LPGAMEOBJECT gameObject);
	void Destroy_Visual(LPGAMEOBJECT gameObject);
	void AddToCell(int cell, LPGAMEOBJECT gameObject);
	void RemoveFromCell(int cell, LPGAMEOBJECT gameObject);

	LPGAMEOBJECT GetMario() { return this->mario; }
	void GetMarioPos(float& x, float& y);
	void FloatText(float x, float y);
	void FloatTextCoin(float x, float y);
	void FloatTextCustom(float x, float y, int point);
	void FloatEffectSplash(float x, float y);
	void SortGameObject();

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	
	//CMario* GetPlayer() { return player; }

};
typedef TestScene* LPTESTSCENE;

