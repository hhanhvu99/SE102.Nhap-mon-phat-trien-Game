#include "ActiveBlock.h"
#include "debug.h"

ActiveBlock::ActiveBlock(float x, float y, int id, LPSPRITE sprite)
{
	this->x = x;
	this->y = y;
	this->sprite = sprite;
	this->hp = 1;
	
}
