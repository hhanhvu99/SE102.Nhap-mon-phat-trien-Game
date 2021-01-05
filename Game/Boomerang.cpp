#include "Boomerang.h"

Boomerang::Boomerang(float x, float y, int direction, LPGAMEOBJECT thrower)
{
	this->x = x;
	this->y = y;

	this->vx = direction * BULLET_BOOMERANG_START_VX;
	this->vy = -BULLET_BOOMERANG_START_VY;

	this->width = BULLET_BOOMERANG_WIDTH;
	this->height = BULLET_BOOMERANG_HEIGHT;
	this->direction = direction;

	this->thrower = thrower;
	this->draw_order = BULLET_DRAW_ORDER;
	this->bulletType = BULLET_BOOMERANG_TYPE;
	this->type = eType::ENEMY_BULLET;
	this->state = BULLET_STATE_MOVING;
	this->animation_set = AnimationManager::GetInstance()->Get(BULLET);

	this->Add();
}

void Boomerang::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Add(this);
	mario = current->GetMario();

}

void Boomerang::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);

	current->Destroy(this);

}

void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + this->width;
	bottom = y + this->height;
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// Simple fall down
	if (PAUSE == false)
	{
		thrower->GetPosition(throwerX, throwerY);

		x += dx;
		y += dy;

		if (y > throwerY + BULLET_BOOMERANG_OFFSET_Y)
			vy = 0;
		else
			vy += BULLET_BOOMERANG_REFLECT_VY * dt;

		if (direction > 0 && vx < 0)
		{
			if (x < throwerX)
			{
				this->Destroy();
				return;
			}
		}
		else if (direction < 0 && vx > 0)
		{
			if (x > throwerX)
			{
				this->Destroy();
				return;
			}
		}

		vx += -direction * BULLET_BOOMERANG_REFLECT_VX * dt;

		CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY, eType::ENEMY_BULLET, eType::ENEMY_MOB_DIE, eType::P_BLOCK,
			eType::BLOCK, eType::GROUP, eType::BRICK, eType::QUESTION, eType::PLAYER_UNTOUCHABLE, eType::PLATFORM , eType::ITEM, eType::GROUP_MOVING });

	}
	else
	{
		vx = 0;
		vy = 0;
		return;
	}

	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);
	if (state == BULLET_STATE_IDLE)
	{
		vx = 0;
		vy = 0;
	}

	// No collision occured, proceed normally
	//DebugOut(L"Size: %d \n", coEvents.size());
	//DebugOut(L"Result: %d\n", result);

	if (coEvents.size() != 0)
	{
		float min_tx, min_ty;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		//DebugOut(L"vx: %f -- vy: %f\n", vx, vy);
		//DebugOut(L"nx: %f -- ny: %f", nx, ny);


		//
		// Collision logic with other objects
		//

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == eType::PLAYER)
				e->obj->SetState(MARIO_STATE_HIT);

		}

	}

	//DebugOut(L"x: %f -- y:%f\n", x, y);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
		coEvents[i] = NULL;
	}

	if (x < camPosX - BULLET_SAFE_DELETE_RANGE || x > camPosX + SCREEN_WIDTH + BULLET_SAFE_DELETE_RANGE ||
		y < camPosY - BULLET_SAFE_DELETE_RANGE || y > camPosY + SCREEN_HEIGHT + BULLET_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}
}

void Boomerang::Render()
{
	int ani = -1;

	if (direction > 0) ani = bulletType + BULLET_ANI_RIGHT;
	else ani = bulletType + BULLET_ANI_LEFT;

	animation_set->Get(ani)->Render(x, y);
	RenderBoundingBox();

}

void Boomerang::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();

	switch (state)
	{
	case BULLET_STATE_IDLE:
		break;
	case BULLET_STATE_MOVING:
		break;
	}
}

Boomerang::~Boomerang()
{
	animation_set = NULL;
	sprite = NULL;

}
