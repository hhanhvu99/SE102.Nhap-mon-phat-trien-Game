#pragma once

#include "Scene.h"
#include "Keyboard.h"

#include "Block.h"
#include "BackGround.h"
#include "GroupObject.h"
#include "BrickShiny.h"
#include "QuestionBlock.h"
#include "Groomba.h"
#include "Koopas.h"
#include "Plant.h"
#include "SuperLeaf.h"
#include "SuperStar.h"
#include "Mushroom.h"
#include "Coin.h"
#include "EnemyTroop.h"
#include "Teleport.h"

#include "HUD.h"

#include "Global.h"

#define FLOAT_TEXT_HEIGHT		8.0f

using namespace std;

class TestScene: public Scene
{
protected:
	GLOBAL global;
	LPGAMEOBJECT mario;
	vector<LPGAMEOBJECT> gameObjects;
	vector<LPGAMEOBJECT> collideObjects;
	vector<LPGAMEOBJECT> deleteList;

	vector<LPGAMEOBJECT> teleport;
	LPGAMEOBJECT currentGate;

	int combo = 0;
	int soLanUpdate = 0;
	int currentWorld;

public:

	TestScene(int id, LPCWSTR filePath);

	void Add(LPGAMEOBJECT gameObject);
	void Destroy(LPGAMEOBJECT gameObject);
	void Remove(LPGAMEOBJECT gameObject);
	void Add_Visual(LPGAMEOBJECT gameObject);
	void Destroy_Visual(LPGAMEOBJECT gameObject);

	LPGAMEOBJECT GetMario() { return this->mario; }
	void GetMarioPos(float& x, float& y);
	void FloatText(float x, float y);
	void FloatTextCoin(float x, float y);
	void SortGameObject();

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	
	//CMario* GetPlayer() { return player; }

};
typedef TestScene* LPTESTSCENE;

