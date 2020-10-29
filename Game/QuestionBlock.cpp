#include "QuestionBlock.h"
#include "debug.h"

QuestionBlock::QuestionBlock(float x, float y, LPSPRITE sprite)
{
	this->x = oldX = x;
	this->y = oldY = y;
	this->sprite = sprite;
	this->width = this->height = STANDARD_SIZE;

	this->hit = false;
	this->moving = false;
	this->type = eType::QUESTION;
	this->state = QUESTION_BLOCK_STATE_NORMAL;
}

void QuestionBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void QuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	x += dx;
	y += dy;
	
	if (state == QUESTION_BLOCK_STATE_HIT && hit == false)
	{
		oldX = x;
		oldY = y;
		hit = true;
		moving = true;
		startMoving = GetTickCount();
	}

	float distanceX = oldX - x;
	float distanceY = oldY - y;

	if (distanceX + distanceY != 0)
		state = QUESTION_BLOCK_STATE_MOVING;
	else
		state = QUESTION_BLOCK_STATE_NORMAL;

	if (moving)
		vy = -MOVING_SPEED;

	if (state == QUESTION_BLOCK_STATE_MOVING)
	{
		if (GetTickCount() - startMoving > BLOCK_MOVING_TIME)
		{
			moving = false;
		}

		if (moving == false)
		{
			if (abs(distanceX) >= BOUNDARY)
			{
				vx = Global::Sign(distanceX) * DEFLECT_SPEED;
			}
			else
			{
				vx = 0;
				x = oldX;
			}
			if (abs(distanceY) >= BOUNDARY)
			{
				vy = Global::Sign(distanceY) * DEFLECT_SPEED;
			}
			else
			{
				vy = 0;
				y = oldY;
			}
		}
		
	}

	//DebugOut(L"State Update: %d \n", state);
}

void QuestionBlock::Render()
{
	if (!hit)
	{
		int ani = QUESTION_BLOCK_ANI;
		animation_set->Get(ani)->Render(x, y);
	}
	else
	{
		sprite->Draw(x, y);
	}

	//RenderBoundingBox();

}
