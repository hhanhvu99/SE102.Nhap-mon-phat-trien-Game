#include "SuperStar.h"

SuperStar::SuperStar(float x, float y, int itemType) : Item(x, y, itemType)
{
	this->boundaryUp = this->y - STANDARD_SIZE;
	this->showUp = false;

	this->Add();
}

void SuperStar::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void SuperStar::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	Global::GetInstance()->point += 1000;

	current->Destroy(this);

}

void SuperStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
			vy += ITEM_GRAVITY * dt;

		CalcPotentialCollisions(coObjects, coEvents, { eType::ITEM, eType::ENEMY, eType::ENEMY_BULLET,
					eType::PLAYER_UNTOUCHABLE, eType::ENEMY_MOB_DIE });

		if (state == ITEM_STATE_MOVING)
		{
			vx = direction * ITEM_MOVE_SPEED_X;
		}
		else if (state == ITEM_STATE_SHOW)
		{
			if (this->y <= boundaryUp)
			{
				showUp = false;
				state = ITEM_STATE_MOVING;
				type = eType::ITEM;
			}
			else
			{
				vy = -ITEM_MOVE_SPEED_Y;
			}

		}

	}
	else
	{
		vx = 0;
		vy = 0;
		return;
	}

	if (state == ITEM_STATE_DROP)
	{
		vx = 0;
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

		this->x += min_tx * dx + nx * 0.4f;
		this->y += min_ty * dy + ny * 0.4f;

		if (ny != 0)
		{
			vy = 0;

			if (ny < 0)
			{
				if (state == ITEM_STATE_DROP)
					state = ITEM_STATE_MOVING;
				else
					vy = -ITEM_JUMP_SPEED;
			}	

		}

		if (nx > 0)
			direction = 1;
		else if (nx < 0)
			direction = -1;

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

	if (x < camPosX - ITEM_SAFE_DELETE_RANGE || x > camPosX + SCREEN_WIDTH + ITEM_SAFE_DELETE_RANGE ||
		y < camPosY - ITEM_SAFE_DELETE_RANGE || y > camPosY + SCREEN_HEIGHT + ITEM_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}
}

void SuperStar::Render()
{
	if (state != ITEM_STATE_IDLE)
	{
		int ani = itemType;

		animation_set->Get(ani)->Render(x, y);
		//RenderBoundingBox();
	}
}

void SuperStar::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case ITEM_STATE_SHOW:
		showUp = true;
		break;
	case ITEM_STATE_HIT:
		this->Destroy();
		break;
	default:
		break;
	}
}

SuperStar::~SuperStar()
{
	animation_set = NULL;
	sprite = NULL;
}
