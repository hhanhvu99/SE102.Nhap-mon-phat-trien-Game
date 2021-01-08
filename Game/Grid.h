#pragma once

#include "TestScene.h"

class Grid : public GameObject
{
	int id;
	float startCell_X, startCell_Y;
	float endCell_X, endCell_Y;

	GLOBAL global;
	LPCWSTR pathToBlock;
	vector<LPGAMEOBJECT> cellObjects;

public:
	Grid(int id, LPCWSTR fileBlock);
	void Init();

	int GetID() { return this->id; }
	void GetCellPos(float& startCellX, float& startCellY, float& endCellX, float& endCellY)
	{
		startCellX = this->startCell_X;
		startCellY = this->startCell_Y;
		endCellX = this->endCell_X;
		endCellY = this->endCell_Y;
	}

	LPGAMEOBJECT FindGroup(int left, int top, int right, int bottom, vector<LPGAMEOBJECT>& collideObjects);
	void FindGroupPos(int& left, int& top, int& right, int& bottom, LPGAMEOBJECT group);
	void Insert(LPGAMEOBJECT objectToAdd);
	void Delete(LPGAMEOBJECT objectToRemove);
	int GetSize() { return this->cellObjects.size(); }

	void Load(vector<LPGAMEOBJECT>& gameObjects, vector<LPGAMEOBJECT>& collideObjects);
	void Unload();

	virtual void Destroy() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Render() {};

	~Grid();
};

typedef Grid* LPGRID;
