#include "Koopas.h"

#include "Koopas.h"
#include "debug.h"

Koopas::Koopas(int placeX, int placeY, int mobType)
{
	this->x = placeX * STANDARD_SIZE;
	this->y = placeY * STANDARD_SIZE - ENEMY_KOOPAS_HEIGHT;

	this->immobilize = false;
	this->rolling = false;
	this->comeBack = false;
	this->shaking = false;
	this->startShaking = false;
	this->beingGrab = false;

	this->width = ENEMY_KOOPAS_WIDTH;
	this->height = ENEMY_KOOPAS_HEIGHT;
	this->type = eType::ENEMY;

	this->state = ENEMY_STATE_MOVING;
	this->direction = 1;
	this->mobType = mobType;

	this->shakeX = ENEMY_KOOPAS_ANI_SHAKE_X;
	this->offsetX = ENEMY_KOOPAS_DRAW_OFFSET_X;
	this->offsetY = ENEMY_KOOPAS_DRAW_OFFSET_Y;

	this->Add();

}

void Koopas::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void Koopas::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Destroy(this);

}

void Koopas::Remove()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Remove(this);
}

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + ENEMY_KOOPAS_BBOX_OFFSET_X;
	top = y;
	right = left + width;
	bottom = top + height;
	//DebugOut(L"left: %f - top: %f - right: %f - bottom: %f\n", left, top, right, bottom);
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// Simple fall down
	if (PAUSE == false)
	{
		vy += ENEMY_KOOPAS_GRAVITY * dt;
		vx = direction * ENEMY_KOOPAS_MOVE_SPEED_X;

		pointX = this->x + width / 2;
		pointY = this->y;
		//DebugOut(L"Grab: %d \n", beingGrab);
		if (type == eType::ENEMY)
		{
			if (beingGrab == true)
				CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY_MOB_DIE, eType::PLAYER_UNTOUCHABLE, eType::PLAYER });
			else if (rolling == false)
				CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY, eType::ENEMY_MOB_DIE, eType::PLAYER_UNTOUCHABLE });
			else if (rolling == true)
				CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY_MOB_DIE, eType::PLAYER_UNTOUCHABLE });
		}
				
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
	else if (immobilize)
	{
		vx = 0;
		vy = 0;

		if (GetTickCount() - timeLeft > ENEMY_KOOPAS_TIME_LEFT)
		{
			state = ENEMY_STATE_MOVING;

			immobilize = false;
			comeBack = false;
			shaking = false;
			startShaking = false;
			
			if (beingGrab)
			{
				beingGrab = false;
				mario->SetState(MARIO_STATE_HIT);
			}

			offsetX = 0;
			//Doi co index
		}
		else if (GetTickCount() - timeLeft > ENEMY_KOOPAS_TIME_COMEBACK)
			comeBack = true;
		else if (GetTickCount() - timeLeft > ENEMY_KOOPAS_TIME_SHAKE)
			shaking = true;

	}
	else if (rolling)
	{
		vx = direction * ENEMY_KOOPAS_ROLL_SPEED_X;
	}
	else if (beingGrab)
	{
		if (mario != NULL)
		{
			vx = 0;
			vy = 0;
		}
		else
			DebugOut(L"[ERROR] No Mario!!!\n");
	}

	// No collision occured, proceed normally
	
	//DebugOut(L"Time left: %d \n", GetTickCount() - timeLeft);
	//DebugOut(L"x: %f\n", offsetX + x);
	//DebugOut(L"vx: %f - vy: %f\n", vx, vy);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		//DebugOut(L"vx: %f -- vy: %f\n", vx, vy);
		//DebugOut(L"nx: %f -- ny: %f -- direction: %d\n", nx, ny, direction);

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
					if (immobilize == false)
						SetState(ENEMY_STATE_STOMP);
					else
						SetState(ENEMY_STATE_ROLLING);
					e->obj->SetSpeed(vx, -MARIO_JUMP_DEFLECT_SPEED);
				}
				else
				{	
					if (immobilize == false)
						e->obj->SetState(MARIO_STATE_HIT);
					else
					{
						Mario* mario = dynamic_cast<Mario*>(e->obj);
						this->mario = mario;

						if (mario->isGrapping())
						{
							mario->SetGrabObject(this);
							mario->SetState(MARIO_STATE_HOLD_SOMETHING);
							beingGrab = true;
						}
						else
						{
							mario->SetState(MARIO_STATE_KICK);
							SetState(ENEMY_STATE_KICK);
							mario->GetDirection(direction);
						}
					}
				}

				break;
			}
			else if (e->obj->GetType() == eType::ENEMY)
			{
				e->obj->SetState(ENEMY_STATE_HIT);
				e->obj->SetDirection(direction);
				break;
			}
			else
			{
				if (nx > 0)
					direction = 1;
				else if (nx < 0)
					direction = -1;
				break;
				
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

	if (x < camPosX - ENTITY_SAFE_DELETE_RANGE * 2 || x > camPosX + ENTITY_SAFE_DELETE_RANGE * 2 ||
		y < camPosY - ENTITY_SAFE_DELETE_RANGE || y > camPosY + ENTITY_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}
}

void Koopas::Render()
{
	int ani = -1;
	if (comeBack)
	{
		ani = mobType + ENEMY_ANI_COMEBACK;
	}
	else if (immobilize)
	{
		ani = mobType + ENEMY_ANI_IMMOBILIZE;
	}
	else if (rolling)
	{
		ani = mobType + ENEMY_ANI_ROLLING;
	}
	else if (state == ENEMY_STATE_MOVING)
	{
		if (direction > 0) ani = mobType + ENEMY_ANI_RIGHT;
		else ani = mobType + ENEMY_ANI_LEFT;
	}
	else if (state == ENEMY_STATE_HIT)
	{
		if (direction > 0) ani = mobType + ENEMY_ANI_DIE_HIT_RIGHT;
		else ani = mobType + ENEMY_ANI_DIE_HIT_LEFT;
	}

	if (shaking)
	{
		if (startShaking == false)
		{
			shakeTime = GetTickCount();
			startShaking = true;
			offsetX = shakeX;
		}
		else if (GetTickCount() - shakeTime > ENEMY_KOOPAS_ANI_SHAKE_TIME)
		{
			startShaking = false;
			offsetX = 0.0f;
		}

	}

	animation_set->Get(ani)->Render(x + offsetX, y + offsetY);
	RenderBoundingBox();
}

void Koopas::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();

	switch (state)
	{
	case ENEMY_STATE_STOMP:
		timeLeft = now;
		comeBack = false;
		shaking = false;
		startShaking = false;
		
		if (immobilize == false)
		{
			immobilize = true;
			rolling = false;

			this->y = this->y - 1.0f;
		
		}

		break;
	case ENEMY_STATE_ROLLING:
		immobilize = false;
		rolling = true;
		comeBack = false;
		shaking = false;
		startShaking = false;
		beingGrab = false;

		break;
	case ENEMY_STATE_KICK:
		SetState(ENEMY_STATE_ROLLING);
		mario->SetPositionBack(min_tx, -nx);
		mario = NULL;

		break;
	case ENEMY_STATE_HIT:
		vy = -ENEMY_KOOPAS_DEFLECT_Y;

		this->type = eType::ENEMY_MOB_DIE;
		this->Remove();
		break;
	}
}
