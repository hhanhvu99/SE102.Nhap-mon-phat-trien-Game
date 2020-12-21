#include "Groomba.h"
#include "debug.h"

Groomba::Groomba(int placeX, int placeY, int mobType, bool hasWing)
{
	this->x = placeX * STANDARD_SIZE;
	this->y = placeY * STANDARD_SIZE - 1.0f;

	this->hitByStomp = false;
	this->hitByBullet = false;
	this->pause = false;
	this->touchGround = true;
	this->countOffGround = 0;
	this->width = ENEMY_GROOMBA_WIDTH;
	this->height = ENEMY_GROOMBA_HEIGHT;
	this->type = eType::ENEMY;
	this->ani_walk_speed = 100;

	this->state = ENEMY_STATE_MOVING;
	this->direction = -1;
	this->mobType = mobType;
	this->hasWing = hasWing;

	if (hasWing)
	{
		numberOfJump = 3;
		allowJump = false;
	}	

	this->offsetX = ENEMY_GROOMBA_DRAW_OFFSET_X;
	this->offsetY = ENEMY_GROOMBA_DRAW_OFFSET_Y;

	this->Add();

}

void Groomba::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void Groomba::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Destroy(this);

}

void Groomba::Remove()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
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
	//Check outside camera
	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);
	if (x < camPosX - ENTITY_SAFE_DELETE_RANGE || x > camPosX + SCREEN_WIDTH + ENTITY_SAFE_DELETE_RANGE ||
		y < camPosY - ENTITY_SAFE_DELETE_RANGE || y > camPosY + SCREEN_HEIGHT + ENTITY_SAFE_DELETE_RANGE)
	{
		if (firstRun)
		{
			lastState = state;
			state = ENEMY_STATE_IDLE;

			timeLeft_dt = GetTickCount() - timeLeft;
			timeJump_dt = GetTickCount() - timeJump;

			firstRun = false;
		}
	}
	else
	{
		if (firstRun == false)
		{
			state = lastState;
			firstRun = true;
		}
	}

	if (state == ENEMY_STATE_IDLE)
	{
		vx = 0;
		vy = 0;

		timeLeft = GetTickCount() - timeLeft_dt;
		timeJump = GetTickCount() - timeJump_dt;

		return;
	}

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

		if (hasWing)
		{
			if (allowJump == false)
			{
				if (numberOfJump == ENEMY_GROOMBA_MAX_JUMP)
				{
					timeJump = GetTickCount();
					numberOfJump = 0;

				}
				else if (GetTickCount() - timeJump > ENEMY_GROOMBA_TIME_JUMP)
					allowJump = true;
			}

		}	

		if (type == eType::ENEMY)
			CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY, eType::ENEMY_BULLET , 
				eType::ENEMY_MOB_DIE, eType::PLAYER_UNTOUCHABLE, eType::ITEM });
	}
	else
	{
		vx = 0;
		vy = 0;
		return;
	}

	if (state == ENEMY_STATE_DROP)
	{
		vx = 0;
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

	// No collision occured, proceed normally
	//DebugOut(L"Size: %d \n", coEvents.size());
	//DebugOut(L"x: %f - y: %f\n", x, y);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (++countOffGround > 3)
		{
			touchGround = false;
		}

		if (vy >= 0)
			ani_walk_speed = 100;
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


		if (ny != 0)
		{
			vy = 0;

			if (ny < 0)
			{
				countOffGround = 0;
				touchGround = true;

				if (hasWing)
				{
					if (allowJump)
					{
						if (numberOfJump == ENEMY_GROOMBA_MAX_JUMP - 1)
						{
							ani_walk_speed = 40;
							vy = -ENEMY_GROOMBA_JUMP_LONG;
						}
						else
						{
							vy = -ENEMY_GROOMBA_JUMP_SHORT;
						}

						numberOfJump += 1;

						if (numberOfJump == ENEMY_GROOMBA_MAX_JUMP)
							allowJump = false;
					}

				}
			}

		}
			

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == eType::PLAYER)
			{
				float vx, vy;
				e->obj->GetSpeed(vx, vy);

				
				if (e->ny > 0)
				{
					if (hasWing)
					{
						hasWing = false;

						LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
						LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
						current->FloatText(x, y);
					}
					else
						SetState(ENEMY_STATE_STOMP);
					e->obj->SetSpeed(vx, -MARIO_JUMP_DEFLECT_SPEED);

					this->y -= min_ty * dy + ny * 0.4f;
				}
				else
				{
					e->obj->SetState(MARIO_STATE_HIT);
				}

			}
			else
			{
				if (nx > 0)
					direction = 1;
				else if (nx < 0)
					direction = -1;
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
	if (firstRun == false)
		return;

	int ani = -1;
	
	if (hasWing)
	{
		if (state == ENEMY_STATE_MOVING)
		{
			if (touchGround == false)
			{
				ani = mobType + ENEMY_ANI_WING_JUMP;
			}
			else
			{
				if (direction > 0) ani = mobType + ENEMY_ANI_RIGHT_WING;
				else ani = mobType + ENEMY_ANI_LEFT_WING;
			}
			animation_set->Get(ani)->SetTime(ani_walk_speed);
		}
		else if (state == ENEMY_STATE_HIT || state == ENEMY_STATE_HIT_TAIL)
		{
			if (direction > 0) ani = mobType + ENEMY_ANI_DIE_HIT_RIGHT;
			else ani = mobType + ENEMY_ANI_DIE_HIT_LEFT;
		}
	}
	else
	{
		if (state == ENEMY_STATE_DROP)
		{
			ani = mobType + ENEMY_ANI_IDLE;
		}
		else if (state == ENEMY_STATE_MOVING)
		{
			if (direction > 0) ani = mobType + ENEMY_ANI_RIGHT;
			else ani = mobType + ENEMY_ANI_LEFT;
		}
		else if (state == ENEMY_STATE_STOMP)
		{
			ani = mobType + ENEMY_ANI_DIE_STOMP;
		}
		else if (state == ENEMY_STATE_HIT || state == ENEMY_STATE_HIT_TAIL)
		{
			if (direction > 0) ani = mobType + ENEMY_ANI_DIE_HIT_RIGHT;
			else ani = mobType + ENEMY_ANI_DIE_HIT_LEFT;
		}
	}

	
	animation_set->Get(ani)->Render(x + offsetX, y + offsetY);
	//RenderBoundingBox();
}

void Groomba::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();

	switch (state)
	{
	case ENEMY_STATE_STOMP:
	{
		pause = true;
		timeLeft = now;
		this->type = eType::ENEMY_MOB_DIE;

		if (CHOOSE != 2)
		{
			LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
			LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
			current->FloatText(x, y);
		}
		
	}	

		break;
	case ENEMY_STATE_HIT:
	case ENEMY_STATE_HIT_TAIL:
	{
		vy = -ENEMY_GROOMBA_DEFLECT_Y;

		this->type = eType::ENEMY_MOB_DIE;
		this->Remove();

		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
		current->FloatText(x, y);
	}	

		break;
	}
}
