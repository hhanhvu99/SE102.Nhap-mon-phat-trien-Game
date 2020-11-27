#include "ActiveBlock.h"
#include "debug.h"

ActiveBlock::ActiveBlock(float x, float y, LPSPRITE sprite)
{
	this->sprite = sprite;
	this->item = NULL;
}
