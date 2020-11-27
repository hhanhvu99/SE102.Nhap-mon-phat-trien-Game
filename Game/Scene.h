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

	KeyEventHandler* key_handler;
	int id;
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
};
typedef Scene* LPSCENE;


class ScenceKeyHandler : public KeyEventHandler
{
protected:
	Scene* scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	ScenceKeyHandler(Scene* s) :KeyEventHandler() { scence = s; }
};