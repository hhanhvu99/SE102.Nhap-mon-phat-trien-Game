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

void Sprite::Draw(float x, float y, D3DCOLOR color, float angle, float offsetX, float offsetY)
{
	GameEngine* game = GameEngine::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, color, this->angle, this->offsetX, this->offsetY);
	
}