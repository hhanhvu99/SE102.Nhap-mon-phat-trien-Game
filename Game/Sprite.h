#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

class Sprite
{
	int id;				// Sprite ID in the sprite database

	float angle;
	float offsetX;
	float offsetY;
	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, float angle = 0.0f, float offsetX = 0.0f, float offsetY = 0.0f);
	void SetDirection(int direction);
	void SetOffset(float x, float y);
	void Draw(float x, float y, D3DCOLOR color = D3DCOLOR_ARGB(255,255,255,255), float angle = 0, float offsetX = 0, float offsetY = 0);
};

typedef Sprite* LPSPRITE;

