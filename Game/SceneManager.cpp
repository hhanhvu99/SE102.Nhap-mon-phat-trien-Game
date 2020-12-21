#include <iostream>
#include <fstream>

#include "SceneManager.h"
#include "debug.h"

SceneManager* SceneManager::__instance = NULL;

/*
	Load game campaign file and load/initiate first scene
*/

#define BLOCK_WIDTH 16
#define BLOCK_HEIGHT 16

#define MAX_GAME_LINE 1024
#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SIZE 1
#define GAME_FILE_SECTION_SCENES 2

void SceneManager::_ParseSection_SIZE(string line, int id)
{
	vector<string> tokens = split(line, " ");

	if (tokens.size() < 2)
	{
		DebugOut(L"[ERROR] Unknown map size %s\n");
		return;
	}
		
	scenes[id]->width = atoi(tokens[0].c_str());
	scenes[id]->height = atoi(tokens[1].c_str());
}

void SceneManager::_ParseSection_SCENES(string line, int id, int lineNumber)
{
	vector<string> tokens = split(line, ",");

	if (tokens.size() < 2) 
		return;

	int value, i;
	int width = scenes[id]->width;
	int height = scenes[id]->height;

	for (i = 0; i < width; ++i)
	{
		value = atoi(tokens[i].c_str());

		scenes[id]->map[i][lineNumber] = value;

		//DebugOut(L"%d ", scenes[id]->map[i][lineNumber]);
	}
	//DebugOut(L"\n");
}

void SceneManager::Load(LPSCENE scene)
{
	LPCWSTR gameFile = scene->GetFilePath();
	int sceneID = scene->GetID();
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	scenes[sceneID] = scene;
	current_scene = sceneID;

	ifstream f;
	f.open(gameFile);
	
	int section = GAME_FILE_SECTION_UNKNOWN;

	char str[MAX_GAME_LINE];
	int lineNumber = -1;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#')
			continue;

		if (line == "[Size]") { section = GAME_FILE_SECTION_SIZE; continue; }
		if (line == "[Grid]") { section = GAME_FILE_SECTION_SCENES; continue; }

		switch (section)
		{
			case GAME_FILE_SECTION_SIZE: _ParseSection_SIZE(line, sceneID); break;
			case GAME_FILE_SECTION_SCENES: 
				++lineNumber;
				_ParseSection_SCENES(line, sceneID, lineNumber); 
				break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);
}

void SceneManager::SwitchScene(int scene_id)
{
	DebugOut(L"[INFO] Switching to scene %d\n", scene_id);

	scenes[current_scene]->Unload();

	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];

	s->Load();

	if (s->GetID() != SCENE_INTRO)
		s->SetSwitch();
}

SceneManager* SceneManager::GetInstance()
{
	if (__instance == NULL) __instance = new SceneManager();
	return __instance;
}