#include "HUD_Object.h"

HUD_Object::HUD_Object(LPSPRITE sprite)
{
	this->sprite = sprite;
}

void HUD_Object::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void HUD_Object::Render()
{
	sprite->Draw(x, y);
}
