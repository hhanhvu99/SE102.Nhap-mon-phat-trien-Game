#pragma once

#include <vector>
#include <d3d9.h>
#include "KeyEventHandler.h"

class Scene
{
protected:
	std::vector<int> BLOCKS;
	std::vector<int> PLATFORM;
	std::vector<int> ACTIVE_BLOCKS;
	std::vector<int> GROUP;
	std::vector<int> ENEMY;
	std::vector<int> ITEM;
	std::vector<int> PATH;
	std::vector<int> START;
	std::vector<int> GATE;
	std::vector<int> COLOR;
	std::vector<int> P_BLOCK_HOLDER;

	KeyEventHandler* key_handler;

	int id;
	int state;
	int type;
	D3DCOLOR backgroundColor;

	LPCWSTR sceneFilePath;

	bool startSwitch = false;

public:
	int width, height;
	int map[500][500];
	float startPosX, startPosY;

	Scene(int id, LPCWSTR filePath);

	KeyEventHandler* GetKeyEventHandler() { return key_handler; }
	LPCWSTR GetFilePath();
	int GetID();

	void SetSwitch() { startSwitch = true; }
	void LoadBlock(LPCWSTR gameFile);
	void SetStartPos(float x, float y) { startPosX = x; startPosY = y; }

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	~Scene();
};
typedef Scene* LPSCENE;
