#pragma once

#include "ActiveBlock.h"
#include "Rubbish.h"
#include "P_Block.h"

#define RUBBISH_OFFSET_X	3.0f
#define RUBBISH_OFFSET_Y	3.0f

class BrickShiny : public ActiveBlock
{
	int countDestroy;
	float oldX, oldY;
	bool moving, destroy;

	DWORD startMoving;

	LPGAMEOBJECT master;

public:
	BrickShiny(float x, float y, LPSPRITE sprite = NULL);

	virtual void Destroy();
	void SetMaster(LPGAMEOBJECT master) { this->master = master; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state); 

	~BrickShiny();
};
