﻿#include "SuperLeaf.h"

SuperLeaf::SuperLeaf(float x, float y, int itemType) : Item(x, y, itemType)
{
	this->showUp = false;
	this->stop = true;
	this->leafBreak = false;
	this->leafBreak_Y = false;

	this->Add();
}

/*
	Chạm vào thì hủy
*/
void SuperLeaf::DestroyTouch()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	//Thêm 1000 điểm
	current->FloatTextCustom(x, y, HUD_BONUS_POINT_1000);

	current->Destroy(this);

}

void SuperLeaf::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void SuperLeaf::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	current->Destroy(this);

}

void SuperLeaf::Replace()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	current->Destroy(this);
}

void SuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (state == ITEM_STATE_IDLE)
	{
		vx = 0;
		vy = 0;
		return;
	}

	// Simple fall down
	if (PAUSE == false)
	{
		if (showUp == false)	
			vy += ITEM_LEAF_SPEED_Y * dt;

		//Item di chuyển thì lá di chuyển trái phải liên tục
		if (state == ITEM_STATE_MOVING)
		{
			//Đổi hướng
			if (stop)
			{
				direction = -direction;

				vx = 0;
				vy = 0;

				stop = false;
				leafBreak = false;
			}
			//TH còn lại, tùy vào hướng mà lá sẽ thay đổi khi tốc độ vượt quá một ngưỡng nào đó
			else
			{
				if (direction > 0)
				{
					if (leafBreak)
					{
						vx -= ITEM_FRICTION * dt;
						vy -= ITEM_FRICTION_UP * dt;

						if (vx <= 0)
							stop = true;
					}
					else if (vx >= ITEM_LEAF_BOUNDARY)
					{
						leafBreak = true;
						vy = 0;
					}
					else
						vx += ITEM_LEAF_SPEED_X * dt;
				}
				else
				{
					if (leafBreak)
					{
						vx += ITEM_FRICTION * dt;
						vy -= ITEM_FRICTION_UP * dt;

						if (vx >= 0)
							stop = true;
					}
					else if (vx <= -ITEM_LEAF_BOUNDARY)
					{
						leafBreak = true;
						vy = 0;
					}
					else
						vx += -ITEM_LEAF_SPEED_X * dt;
				}

			}
			
		}
		//Item xuất hiện từ block thì item nhảy lên
		else if (state == ITEM_STATE_SHOW)
		{
			if (vy > 0)
			{
				showUp = false;
				state = ITEM_STATE_MOVING;
				type = eType::ITEM;

				SetDrawOrder(BULLET_DRAW_ORDER);
			}
			else
				vy += ITEM_GRAVITY * dt;

		}

	}
	else
	{
		vx = 0;
		vy = 0;
		return;
	}

	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);

	// No collision occured, proceed normally
	//DebugOut(L"Size: %d \n", coEvents.size());
	//DebugOut(L"y: %f\n", this->y);

	x += dx;
	y += dy;

	//Ra khỏi camera thì xóa
	if (x < camPosX - ITEM_SAFE_DELETE_RANGE || x > camPosX + SCREEN_WIDTH + ITEM_SAFE_DELETE_RANGE ||
		y < camPosY - ITEM_SAFE_DELETE_RANGE || y > camPosY + SCREEN_HEIGHT + ITEM_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}
}

void SuperLeaf::Render()
{
	//Ani thuộc ITEM_ID
	if (state != ITEM_STATE_IDLE)
	{
		int ani = -1;

		if (direction > 0) ani = itemType + ITEM_ANI_RIGHT;
		else ani = itemType + ITEM_ANI_LEFT;

		animation_set->Get(ani)->Render(x, y);
		//RenderBoundingBox();
	}
}

void SuperLeaf::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case ITEM_STATE_SHOW:
		showUp = true;
		vy = -ITEM_JUMP_SPEED;
		break;
	case ITEM_STATE_HIT:
		this->DestroyTouch();
		break;
	default:
		break;
	}
}

SuperLeaf::~SuperLeaf()
{
	animation_set = NULL;
	sprite = NULL;
}
