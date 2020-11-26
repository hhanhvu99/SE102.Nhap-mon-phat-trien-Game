#pragma once

#include "Scene.h"

#include "Keyboard.h"

#include "Block.h"
#include "BackGround.h"
#include "Mario.h"
#include "GroupObject.h"
#include "BrickShiny.h"
#include "QuestionBlock.h"
#include "Groomba.h"
#include "Koopas.h"
#include "Plant.h"

#include "Global.h"

using namespace std;

class Mario;
class TestScene: public Scene
{
protected:
	Mario* mario;
	vector<LPGAMEOBJECT> gameObjects;
	vector<LPGAMEOBJECT> collideObjects;

public:

	TestScene(int id, LPCWSTR filePath);
	void Add(LPGAMEOBJECT gameObject);
	void Destroy(LPGAMEOBJECT gameObject);
	void Remove(LPGAMEOBJECT gameObject);
	Mario* GetMario() { return this->mario; }
	void GetMarioPos(float& x, float& y);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	
	//CMario* GetPlayer() { return player; }

};
typedef TestScene* LPTESTSCENE;

