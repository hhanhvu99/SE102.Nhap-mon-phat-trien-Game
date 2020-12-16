#include "EndGoal.h"

EndGoal::EndGoal(float x, float y) : GameObject()
{
	this->x = x;
	this->y = y;
	this->width = STANDARD_SIZE;
	this->height = STANDARD_SIZE;
	this->state = GOAL_STATE_NORMAL;

	this->type = eType::GOAL;
	this->currentItem = 2;

}

void EndGoal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void EndGoal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	x += dx;
	y += dy;

	if (state == GOAL_STATE_NORMAL)
	{
		currentItem += 1;

		if (currentItem > 4)
			currentItem = 2;
	}
	else if (state = GOAL_STATE_HIT)
	{
		vy = -END_GOAL_ITEM_MOVE_SPEED;

		float cx, cy;
		GameEngine::GetInstance()->GetCamPos(cx, cy);

		if (y + height < cy)
			state = GOAL_STATE_IDLE;
	}
	else
	{
		vx = 0;
		vy = 0;
	}
}

void EndGoal::Render()
{
	int ani = -1;

	if (state == GOAL_STATE_NORMAL)
	{
		ani = currentItem + END_GOAL_ANI_ID;
		SpriteManager::GetInstance()->Get(ani)->Draw(x, y);
	}
	else
	{
		if (currentItem == END_GOAL_STAR)
			ani = STAR_ANI_GOAL;
		else if (currentItem == END_GOAL_FLOWER)
			ani = FLOWER_ANI_GOAL;
		else
			ani = MUSHROOM_ANI_GOAL;

		animation_set->Get(ani)->Render(x, y);
	}

}

