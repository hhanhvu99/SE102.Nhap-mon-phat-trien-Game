#pragma once

#include "GameObject.h"
#include <wincrypt.h>

#define FONT_WIDTH			8.0f
#define FONT_HEIGHT			8.0f

class HUD_Object : public GameObject
{
	bool setup;
	int limit;
	string text;
	string delimiter = " ";
	vector<string> tokens;

public:
	HUD_Object(LPSPRITE sprite);
	HUD_Object(string text, int limit);

	void SetText(string text);
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }
	LPSPRITE GetSprite() { return this->sprite; }
	void GetToken(string text);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
};

