#pragma once

#include <vector>
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

	KeyEventHandler* key_handler;
	int id;
	int state;
	int type;
	LPCWSTR sceneFilePath;

public:
	int width, height;
	int map[500][500];

	Scene(int id, LPCWSTR filePath);

	KeyEventHandler* GetKeyEventHandler() { return key_handler; }
	LPCWSTR GetFilePath();
	int GetID();

	void LoadBlock(LPCWSTR gameFile);

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	~Scene();
};
typedef Scene* LPSCENE;
