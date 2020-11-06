#include "Groomba.h"
#include "debug.h"

Groomba::Groomba(int placeX, int placeY, int mobType)
{
	this->x = placeX * STANDARD_SIZE;
	this->y = placeY * STANDARD_SIZE - 10.0f;

	this->hitByStomp = false;
	this->hitByBullet = false;
	this->pause = false;
	this->width = ENEMY_GROOMBA_WIDTH;
	this->height = ENEMY_GROOMBA_HEIGHT;
	this->type = eType::ENEMY;

	this->state = ENEMY_STATE_MOVING;
	this->direction = 1;
	this->mobType = mobType;

	this->offsetX = ENEMY_GROOMBA_DRAW_OFFSET_X;
	this->offsetY = ENEMY_GROOMBA_DRAW_OFFSET_Y;

	this->Add();

}

void Groomba::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void Groomba::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Destroy(this);

}

void Groomba::Remove()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Remove(this);
}

void Groomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + ENEMY_GROOMBA_BBOX_OFFSET_X;
	top = y;
	right = left + ENEMY_GROOMBA_WIDTH;
	bottom = top + ENEMY_GROOMBA_HEIGHT;

}

void Groomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// Simple fall down
	if (PAUSE == false)
	{
		vy += ENEMY_GROMMBA_GRAVITY * dt;
		vx = direction * ENEMY_GROOMBA_MOVE_SPEED_X;

		pointX = this->x + width/2;
		pointY = this->y;

		if (type == eType::ENEMY)
			CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY, eType::PLAYER_UNTOUCHABLE });
	}
	else
	{
		vx = 0;
		vy = 0;
		return;
	}

	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);
	if (state == ENEMY_STATE_IDLE)
	{
		vx = 0;
		vy = 0;
	}
	else if (state == ENEMY_STATE_STOMP)
	{
		vx = 0;
		vy = 0;

		if (GetTickCount() - timeLeft > ENEMY_GROOMBA_TIME_LEFT)
		{
			this->Destroy();
			//Doi co index
			return;
		}
	}
	else if (x < camPosX - ENTITY_SAFE_DELETE_RANGE*2 || x > camPosX + ENTITY_SAFE_DELETE_RANGE*2 ||
		y < camPosY - ENTITY_SAFE_DELETE_RANGE || y > camPosY + ENTITY_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}

	// No collision occured, proceed normally
	//DebugOut(L"Size: %d \n", coEvents.size());
	//DebugOut(L"x: %f - y: %f\n", x, y);

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
		//DebugOut(L"nx: %f -- ny: %f\n", nx, ny);

		this->x += min_tx * dx + nx * 0.4f;
		this->y += min_ty * dy + ny * 0.4f;


		if (vy != 0)
			vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == eType::PLAYER)
			{
				float vx, vy;
				e->obj->GetSpeed(vx, vy);

				
				if (e->ny > 0)
				{
					SetState(ENEMY_STATE_STOMP);
					e->obj->SetSpeed(vx, -MARIO_JUMP_DEFLECT_SPEED);
				}
				else
				{
					e->obj->SetState(MARIO_STATE_HIT);
				}

				break;
			}
			else
			{
				if (nx != 0)
				{	
					SetState(ENEMY_STATE_INVERSE);
					break;
				}
			}
		}
		
	}

	//DebugOut(L"x: %f -- y:%f\n", x, y);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); ++i)
	{
		delete coEvents[i];
		coEvents[i] = NULL;
	}

}

void Groomba::Render()
{
	int ani = -1;
	
	if (state == ENEMY_STATE_MOVING)
	{
		if (direction > 0) ani = mobType + ENEMY_ANI_RIGHT;
		else ani = mobType + ENEMY_ANI_LEFT;
	}
	else if (state == ENEMY_STATE_STOMP)
	{
		ani = mobType + ENEMY_ANI_DIE;
	}
	else if (state == ENEMY_STATE_HIT)
	{
		if (direction > 0) ani = mobType + ENEMY_ANI_DIE_HIT_RIGHT;
		else ani = mobType + ENEMY_ANI_DIE_HIT_LEFT;
	}

	
	animation_set->Get(ani)->Render(x + offsetX, y + offsetY);
	RenderBoundingBox();
}

void Groomba::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();

	switch (state)
	{
	case ENEMY_STATE_IDLE:
		pause = true;
		break;
	case ENEMY_STATE_MOVING:
		pause = false;
		break;
	case ENEMY_STATE_STOMP:
		pause = true;
		timeLeft = now;
		this->type = eType::ENEMY_MOB_DIE;
		break;
	case ENEMY_STATE_INVERSE:
		direction = -direction;
		this->state = ENEMY_STATE_MOVING;
		break;
	case ENEMY_STATE_HIT:
		vy = -ENEMY_GROOMBA_DEFLECT_Y;

		this->type = eType::ENEMY_MOB_DIE;
		this->Remove();
		break;
	}
}
