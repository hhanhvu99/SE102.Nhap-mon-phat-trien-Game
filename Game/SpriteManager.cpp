#include <fstream>

#include "SpriteManager.h"
#include "debug.h"

#define MAX_GAME_LINE 1024

SpriteManager* SpriteManager::__instance = NULL;

SpriteManager* SpriteManager::GetInstance()
{
	if (__instance == NULL) __instance = new SpriteManager();
	return __instance;
}

void SpriteManager::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, float angle, float offsetX, float offsetY)
{
	if (sprites.find(id) != sprites.end())
	{
		sprites[id]->SetTexture(tex);
		sprites[id]->SetRegion(left, top, right, bottom);
		sprites[id]->SetAngle(angle);
		sprites[id]->SetOffset(offsetX, offsetY);
	}
	else
	{
		LPSPRITE s = new Sprite(id, left, top, right, bottom, tex, angle, offsetX, offsetY);
		sprites[id] = s;
	}
	
	//DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

void SpriteManager::Load(LPCWSTR gameFile, LPDIRECT3DTEXTURE9 tex)
{
	ifstream f;
	f.open(gameFile);

	char str[MAX_GAME_LINE];
	int lineNumber = -1;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '/') { continue; }

		ParseCommon(line, tex);
	}
	f.close();

	DebugOut(L"[INFO] Loading game sprites : %s has been loaded successfully\n", gameFile);
}

LPSPRITE SpriteManager::Get(int id)
{
	return sprites[id];
}

/*
	Clear all loaded textures
*/

void SpriteManager::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}

void SpriteManager::ParseCommon(string line, LPDIRECT3DTEXTURE9 tex)
{
	vector<string> tokens = split(line, ",");
	vector<int> data;

	if (tokens.size() < 2)
		return;

	data.clear();

	for (unsigned int i = 0; i < tokens.size(); ++i)
		data.push_back(atoi(tokens[i].c_str()));

	if (data.size() <= 5)
		this->Add(data[0], data[1], data[2], data[3], data[4], tex);
	else if (data.size() == 6)
		this->Add(data[0], data[1], data[2], data[3], data[4], tex, float(data[5]));
	else if (data.size() == 7)
		this->Add(data[0], data[1], data[2], data[3], data[4], tex, float(data[5]), float(data[6]));
	else
		this->Add(data[0], data[1], data[2], data[3], data[4], tex, float(data[5]), float(data[6]), float(data[7]));
}

