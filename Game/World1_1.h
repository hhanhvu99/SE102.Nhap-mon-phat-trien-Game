#pragma once

#include "TestScene.h"

class World1_1 : public TestScene
{
	Mario* castMario;
	bool startSwitching;
	bool stillSwitching;

public:
	World1_1(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void SetState(int state);
};

