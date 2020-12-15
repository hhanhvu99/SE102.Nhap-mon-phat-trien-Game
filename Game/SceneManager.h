#pragma once

#include <unordered_map>

#include "Scene.h"
#include "Texture.h"
#include "SpriteManager.h"
#include "AnimationManager.h"

using namespace std;

class SceneManager
{
protected:
	static SceneManager* __instance;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;

	void _ParseSection_SIZE(string line, int id);
	void _ParseSection_SCENES(string line, int id, int lineNumber);

public:
	void Load(LPSCENE scene);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	static SceneManager* GetInstance();

};

typedef SceneManager* LPSCENEMANAGER;
