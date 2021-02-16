#include "Flower.h"

Flower::Flower(float x, float y, int itemType) : Item(x, y, itemType)
{
	this->boundaryUp = this->y - STANDARD_SIZE + 1.0f;
	this->showUp = false;

	this->Add();
}

/*
	Chạm vào thì hủy
*/
void Flower::DestroyTouch()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	//Nếu mario nhỏ, cộng 1000 điểm, các TH khác mario biến thành mario nhỏ
	if (Global::GetInstance()->level == MARIO_LEVEL_SMALL)
		current->FloatTextCustom(x, y, HUD_BONUS_POINT_1000);

	current->Destroy(this);
}

void Flower::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void Flower::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	current->Destroy(this);

}

void Flower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
		x += dx;
		y += dy;

		//Đang hiện lên
		if (showUp)
		{
			if (this->y <= boundaryUp)
			{
				showUp = false;
				state = ITEM_STATE_STILL;
				type = eType::ITEM;
			}
			else
			{
				vy = -ITEM_MOVE_SPEED_Y;
			}

		}
		else
		{
			vx = 0;
			vy = 0;
		}

	}
	else
	{
		vx = 0;
		vy = 0;
		return;
	}


	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);

	//Ra khỏi camera thì xóa
	if (x < camPosX - ITEM_SAFE_DELETE_RANGE || x > camPosX + SCREEN_WIDTH + ITEM_SAFE_DELETE_RANGE ||
		y < camPosY - ITEM_SAFE_DELETE_RANGE || y > camPosY + SCREEN_HEIGHT + ITEM_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}
}

void Flower::Render()
{
	//Ani thuộc ITEM_ID
	if (state != ITEM_STATE_IDLE)
	{
		int ani = itemType;

		animation_set->Get(ani)->Render(x, y);
		//RenderBoundingBox();
	}
}

void Flower::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case ITEM_STATE_SHOW:
		showUp = true;
		break;
	case ITEM_STATE_HIT:
		this->DestroyTouch();
		break;
	default:
		break;
	}
}

Flower::~Flower()
{
	animation_set = NULL;
	sprite = NULL;
}
