#include "Item.h"
#include "debug.h"

Item::Item(float x, float y, int itemType)
{
	this->x = x;
	this->y = y;
	this->width = this->height = STANDARD_SIZE;
	this->direction = 1;

	this->type = eType::ENEMY_MOB_DIE;
	this->itemType = itemType;
	this->state = ITEM_STATE_IDLE;

}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}
