#pragma once

#include "TestScene.h"

#define STAGE_OFFSET_TRANSPORT_UP		8.0f
#define STAGE_OFFSET_TRANSPORT_DOWN		8.0f

class World : public TestScene
{
	Mario* castMario;
	bool startSwitching;
	bool stillSwitching;
	DWORD timeCounter = 0;

public:
	World(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void SetState(int state);
};

