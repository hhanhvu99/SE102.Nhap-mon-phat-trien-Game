#include "BackGround.h"

BackGround::BackGround(float x, float y, LPSPRITE sprite)
{
	this->x = x;
	this->y = y;
	this->sprite = sprite;
	this->width = BACKGROUND_BLOCK_WIDTH;
	this->height = BACKGROUND_BLOCK_HEIGHT;
	this->type = eType::BACKGROUND;
}

void BackGround::Render()
{
	this->sprite->Draw(x, y);

}

