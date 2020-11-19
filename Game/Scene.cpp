#include <fstream>

#include "Scene.h"
#include "Global.h"
#include "debug.h"

using namespace std;

Scene::Scene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}

LPCWSTR Scene::GetFilePath()
{
	return this->sceneFilePath;
}

int Scene::GetID()
{
	return this->id;
}

#define MAX_GAME_LINE 1024

void Scene::LoadBlock(LPCWSTR gameFile)
{
	ifstream f;
	f.open(gameFile);

	int option = 0;

	char str[MAX_GAME_LINE];
	BLOCKS.clear();
	ACTIVE_BLOCKS.clear();
	GROUP.clear();
	ENEMY.clear();

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '/') { continue; }
		if (line == "[BLOCK]") { option = 1; continue; }
		if (line == "[ACTIVE BLOCK]") { option = 2; continue; }
		if (line == "[GROUP]") { option = 3; continue; }
		if (line == "[ENEMY]") { option = 4; continue; }
		if (line == "[PLATFORM]") { option = 5; continue; }
		if (option == 1)
		{
			vector<string> tokens = split(line, ",");
			for (int i = 0; i < tokens.size(); ++i)
				BLOCKS.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 2)
		{
			vector<string> tokens = split(line, ",");
			for (int i = 0; i < tokens.size(); ++i)
				ACTIVE_BLOCKS.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 3)
		{
			vector<string> tokens = split(line, ",");
			for (int i = 0; i < tokens.size(); ++i)
				GROUP.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 4)
		{
			vector<string> tokens = split(line, ",");
			for (int i = 0; i < tokens.size(); ++i)
				ENEMY.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 5)
		{
			vector<string> tokens = split(line, ",");
			for (int i = 0; i < tokens.size(); ++i)
				PLATFORM.push_back(atoi(tokens[i].c_str()));
		}
		else
		{
			DebugOut(L"[ERROR] Loading script: %s \n", gameFile);
		}


	}
	f.close();

	DebugOut(L"[INFO] Loading game animations : %s has been loaded successfully\n", gameFile);

}
