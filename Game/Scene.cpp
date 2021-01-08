#include <fstream>

#include "Scene.h"
#include "Global.h"
#include "debug.h"

using namespace std;

Scene::Scene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
	this->cameraMode = 1;
	this->numberOfCell = 1;
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
	this->sceneFileBlock = gameFile;

	ifstream f;
	f.open(gameFile);

	int option = 0;

	char str[MAX_GAME_LINE];
	BLOCKS.clear();
	ACTIVE_BLOCKS.clear();
	PATH.clear();
	START.clear();
	GATE.clear();
	COLOR.clear();
	STAGE_FINISHED.clear();

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '/') { continue; }
		if (line == "[BLOCK]") { option = 1; continue; }
		if (line == "[ACTIVE BLOCK]") { option = 2; continue; }
		if (line == "[PATH]") { option = 7; continue; }
		if (line == "[START]") { option = 8; continue; }
		if (line == "[GATE]") { option = 9; continue; }
		if (line == "[COLOR]") { option = 10; continue; }
		if (line == "[CAMERA MODE]") { option = 13; continue; }
		if (line == "[CELL]") { option = 14; continue; }
		if (option == 1)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				BLOCKS.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 2)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				ACTIVE_BLOCKS.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 7)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				PATH.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 8)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				START.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 9)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				GATE.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 10)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				COLOR.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 13)
		{
			cameraMode = atoi(line.c_str());
		}
		else if (option == 14)
		{
			break;
		}
		else
		{
			DebugOut(L"[ERROR] Loading script: %s \n", gameFile);
		}


	}
	f.close();

	DebugOut(L"[INFO] Loading game animations : %s has been loaded successfully\n", gameFile);

}

Scene::~Scene()
{
	BLOCKS.clear();
	ACTIVE_BLOCKS.clear();
	PATH.clear();
	START.clear();
	GATE.clear();
	COLOR.clear();
	STAGE_FINISHED.clear();
}
