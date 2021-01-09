#include "Coin.h"

Coin::Coin(float x, float y, int itemType, bool outSide) : Item(x, y, itemType)
{
	this->boundaryUp = this->y - STANDARD_SIZE;
	this->outSide = outSide;
	this->master = NULL;

	if (outSide)
	{
		state = ITEM_STATE_MOVING;
		type = eType::ITEM;
	}

	this->Add();
}

void Coin::DestroyTouch()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	if (outSide)
	{
		Global::GetInstance()->money += 1;
		Global::GetInstance()->point += 50;

		if (master != NULL)
			static_cast<P_Block*>(master)->RemoveObject(this);

		current->Destroy(this);
	}
	else
		current->Destroy_Visual(this);

}

void Coin::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	if (outSide)
		current->Add(this);
	else
		current->Add_Visual(this);

}

void Coin::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	if (outSide)
	{
		if (master != NULL)
			static_cast<P_Block*>(master)->RemoveObject(this);

		current->Destroy(this);
	}
	else
		current->Destroy_Visual(this);
		
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (state == ITEM_STATE_IDLE || outSide)
	{
		vx = 0;
		vy = 0;
		return;
	}

	// Simple fall down
	if (PAUSE == false)
	{
		if (!outSide)
		{
			vy += ITEM_COIN_GRAVITY * dt;

			CalcPotentialCollisions(coObjects, coEvents, { eType::ITEM, eType::ENEMY, eType::ENEMY_BULLET,
						eType::PLAYER_UNTOUCHABLE, eType::ENEMY_MOB_DIE });


			if (state == ITEM_STATE_SHOW)
			{
				if (vy > 0)
				{
					state = ITEM_STATE_MOVING;
				}

			}
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

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty;
		float rdx = 0;
		float rdy = 0;
		float nx, ny;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		if (ny != 0)
		{
			SetState(ITEM_STATE_HIT);
			return;

		}

		//DebugOut(L"vx: %f -- vy: %f\n", vx, vy);
		//DebugOut(L"nx: %f -- ny: %f", nx, ny);


	}

	//DebugOut(L"x: %f -- y:%f\n", x, y);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
		coEvents[i] = NULL;
	}
}

void Coin::Render()
{
	if (state != ITEM_STATE_IDLE)
	{
		int ani = itemType;
		animation_set->Get(ani)->Render(x, y);
	}

	//RenderBoundingBox();
}

void Coin::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case ITEM_STATE_SHOW:
		vy = -ITEM_COIN_JUMP_SPEED;
		Global::GetInstance()->money += 1;
		
		break;
	case ITEM_STATE_HIT:
	{
		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
		current->FloatTextCoin(x, y);

		this->DestroyTouch();
	}
		break;
	default:
		break;
	}
}

Coin::~Coin()
{
	animation_set = NULL;
	sprite = NULL;
}
