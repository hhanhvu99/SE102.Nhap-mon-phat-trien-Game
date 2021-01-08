#pragma once

#include <vector>
#include <d3d9.h>
#include "KeyEventHandler.h"

class Scene
{
protected:
	std::vector<int> BLOCKS;
	std::vector<int> ACTIVE_BLOCKS;
	std::vector<int> PATH;
	std::vector<int> START;
	std::vector<int> GATE;
	std::vector<int> COLOR;
	std::vector<int> STAGE_FINISHED;

	KeyEventHandler* key_handler;

	int cameraMode;
	int id;
	int state;
	int type;
	int numberOfCell;
	D3DCOLOR backgroundColor;

	LPCWSTR sceneFilePath;
	LPCWSTR sceneFileBlock;

	bool startSwitch = false;

public:
	int width, height;
	int map[500][500];
	float startPosX, startPosY;

	Scene(int id, LPCWSTR filePath);

	KeyEventHandler* GetKeyEventHandler() { return key_handler; }
	LPCWSTR GetFilePath();

	int GetID();
	int GetType() { return this->type; }
	int GetState() { return this->state; }
	int GetCameraMode() { return this->cameraMode; }
	std::vector<int>& GetBLOCKS() { return this->BLOCKS; }
	std::vector<int>& GetACTIVE_BLOCKS() { return this->ACTIVE_BLOCKS; }

	void SetCellNumber(int numberOfCell) { this->numberOfCell = numberOfCell; }
	void SetSwitch() { startSwitch = true; }
	void DisableSwitch() { startSwitch = false; }
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
