#pragma once
#include "Sprite.h"
#include "Global.h"

class SpriteManager
{
	static SpriteManager* __instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void ParseCommon(string line, LPDIRECT3DTEXTURE9 tex);

	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, float angle = 0.0f, float offsetX = 0.0f, float offsetY = 0.0f);
	void Load(LPCWSTR gameFile, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id);
	void Clear();

	static SpriteManager* GetInstance();
};

typedef SpriteManager* LPSPRITEMANAGER;