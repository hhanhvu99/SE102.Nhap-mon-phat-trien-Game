#include "Block.h"

Block::Block(float x, float y, LPSPRITE sprite)
{
	this->x = x;
	this->y = y;
	this->sprite = sprite;
	this->width = BLOCK_WIDTH;
	this->height = BLOCK_HEIGHT;
	this->type = eType::BLOCK;
}

void Block::Render()
{
	this->sprite->Draw(x, y);
	//RenderBoundingBox();

}

void Block::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;

}
