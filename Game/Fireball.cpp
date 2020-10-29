#include "Fireball.h"

Fireball::Fireball(float x, float y, int direction)
{
	this->x = x;
	this->y = y;
	this->offsetX = BULLET_FIREBALL_DRAW_OFFSET_X;
	this->offsetY = BULLET_FIREBALL_DRAW_OFFSET_Y;
	this->width = BULLET_FIREBALL_WIDTH;
	this->height = BULLET_FIREBALL_HEIGHT;
	this->direction = direction;

	this->bulletType = BULLET_FIREBALL_TYPE_NORMAL;
	this->type = eType::MARIO_BULLET;
	this->state = BULLET_STATE_MOVING;
	this->animation_set = AnimationManager::GetInstance()->Get(BULLET);

	this->Add();
}

void Fireball::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Add(this);
	mario = current->GetMario();

}

void Fireball::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);

	current->Destroy(this);

}

void Fireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + this->width;
	bottom = y + this->height;
}

void Fireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// Simple fall down
	if (pause == false)
	{
		vy += BULLET_FIREBALL_GRAVITY * dt;
		vx = direction * BULLET_FIREBALL_SPEED_X;
		CalcPotentialCollisions(coObjects, coEvents, eType::PLAYER);
	}
	else
	{
		dx = 0;
		dy = 0;
	}
		
	// No collision occured, proceed normally
	//DebugOut(L"Size: %d \n", coEvents.size());
	//DebugOut(L"Result: %d\n", result);

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		//DebugOut(L"vx: %f -- vy: %f\n", vx, vy);
		//DebugOut(L"nx: %f -- ny: %f", nx, ny);


		this->x += min_tx * dx + nx * 0.4f;
		this->y += min_ty * dy + ny * 0.4f;

		if (nx != 0 || ny > 0)
		{
			SetState(BULLET_STATE_HIT);
		}
		else if (ny < 0)
		{
			vy = -BULLET_FIREBALL_DEFLECT_SPEED;
		}
		

		//
		// Collision logic with other objects
		//

		//Đợi enemy
		/*
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			LPGAMEOBJECT obj = e->obj;

			
			
		}*/

	}

	//DebugOut(L"x: %f -- y:%f\n", x, y);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
		coEvents[i] = NULL;
	}
	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);
	if (state == BULLET_STATE_IDLE)
	{
		vx = 0;
		vy = 0;
	}
	else if (state == BULLET_STATE_HIT)
	{
		if (GetTickCount() - timeLeft > BULLET_FIREBALL_TIME_LEFT)
		{
			this->Destroy();
			//Doi co index
		}
	}
	
	else if (x < camPosX - BULLET_SAFE_DELETE_RANGE || x > camPosX + BULLET_SAFE_DELETE_RANGE ||
			 y < camPosY - BULLET_SAFE_DELETE_RANGE || y > camPosY + BULLET_SAFE_DELETE_RANGE)
	{
		this->Destroy();
	}
}

void Fireball::Render()
{
	int ani = -1;
	if (state != BULLET_STATE_HIT)
	{
		if (direction > 0) ani = bulletType + BULLET_ANI_RIGHT;
		else ani = bulletType + BULLET_ANI_LEFT;
	}
	else
	{
		if (direction > 0) ani = bulletType + BULLET_ANI_RIGHT;
		else ani = bulletType + BULLET_ANI_LEFT;
		offsetX = BULLET_EFFECT_DRAW_OFFSET_X;
		offsetY = BULLET_EFFECT_DRAW_OFFSET_Y;
	}

	animation_set->Get(ani)->Render(x + offsetX, y + offsetY);
	//RenderBoundingBox();
	
}

void Fireball::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();

	switch (state)
	{
	case BULLET_STATE_IDLE:
		pause = true;
		break;
	case BULLET_STATE_MOVING:
		pause = false;
		break;
	case BULLET_STATE_HIT:
		pause = true;
		timeLeft = now;
		bulletType = BULLET_EFFECT_POP;
		break;
	}
}

Fireball::~Fireball()
{
	animation_set = NULL;
	sprite = NULL;

}
