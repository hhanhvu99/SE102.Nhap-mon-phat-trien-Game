﻿#include "EndGoal.h"

EndGoal::EndGoal(float x, float y) : GameObject()
{
	this->x = x;
	this->y = y;
	this->width = int(STANDARD_SIZE);
	this->height = int(STANDARD_SIZE);
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

	//Goal cuối cùng chưa chạm
	if (state == GOAL_STATE_NORMAL)
	{
		//Thay đổi item
		if (GetTickCount() - timeStart > END_GOAL_TIME)
		{
			currentItem += 1;
			timeStart = GetTickCount();
		}
		

		if (currentItem > 4)
			currentItem = 2;
	}
	//Nếu chạm goal
	else if (state = GOAL_STATE_HIT)
	{
		//Item di chuyển lên
		vy = -END_GOAL_ITEM_MOVE_SPEED;
		Global::GetInstance()->cardGet = currentItem;
		
		//Chuyển ô bỏ vào nếu ô hiện tại đầy
		if (Global::GetInstance()->currentCardEmpty == 1)
			Global::GetInstance()->cardOne = 30000 + currentItem;
		else if (Global::GetInstance()->currentCardEmpty == 2)
			Global::GetInstance()->cardTwo = 30000 + currentItem;
		else
			Global::GetInstance()->cardThree = 30000 + currentItem;

		//Nếu item ra khỏi màn hình, item đứng yên
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
	//Ani thuộc END_GOAL_ID
	int ani = -1;

	//Bình thường
	if (state == GOAL_STATE_NORMAL)
	{
		ani = currentItem + END_GOAL_ANI_ID;
		SpriteManager::GetInstance()->Get(ani)->Draw(x, y);
	}
	//Ani thay đổi theo item
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

void EndGoal::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	current->Destroy(this);
}

