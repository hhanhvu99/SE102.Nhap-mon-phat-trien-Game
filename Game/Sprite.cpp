#include "Sprite.h"
#include "GameEngine.h"
#include "debug.h"

Sprite::Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, float angle, float offsetX, float offsetY)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->angle = angle;
	this->offsetX = offsetX;
	this->offsetY = offsetY;
	this->texture = tex;
	this->width = right - left;
	this->height = bottom - top;
}

void Sprite::SetDirection(int direction)
{
	if (direction == 1)
		angle = 180;
	else
		angle = 0;
}

void Sprite::SetOffset(float x, float y)
{
	this->offsetX = x;
	this->offsetY = y;
}

void Sprite::SetAngle(float angle)
{
	this->angle = angle;
}

void Sprite::SetRegion(int left, int top, int right, int bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

void Sprite::SetTexture(LPDIRECT3DTEXTURE9 tex)
{
	this->texture = tex;
}

void Sprite::Draw(float x, float y, D3DCOLOR color)
{
	GameEngine* game = GameEngine::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, color, this->angle, this->offsetX, this->offsetY);
	
}

void Sprite::Draw(float x, float y, float angle, D3DCOLOR color)
{
	GameEngine* game = GameEngine::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, color, angle, this->offsetX, this->offsetY);
}
