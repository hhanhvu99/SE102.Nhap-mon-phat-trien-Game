#include "KoopasIntro.h"
#include "debug.h"

KoopasIntro::KoopasIntro(int placeX, int placeY, int mobType, bool inShell)
{
	this->x = placeX * STANDARD_SIZE;
	this->y = placeY * STANDARD_SIZE - ENEMY_KOOPAS_HEIGHT;

	if (inShell)
		SetState(ENEMY_STATE_STOMP);
	else
	{
		state = ENEMY_STATE_MOVING;
		this->immobilize = false;
	}
	
	this->rolling = false;
	this->beingGrab = false;
	this->upSideDown = false;

	this->width = ENEMY_KOOPAS_WIDTH;
	this->height = ENEMY_KOOPAS_HEIGHT;
	this->type = eType::ENEMY;

	this->direction = 1;
	this->mobType = mobType;

	if (mobType == ENEMY_BEETLE)
	{
		this->offsetX = 0;
		this->offsetY = 0;
	}
	else
	{
		this->offsetX = ENEMY_KOOPAS_DRAW_OFFSET_X;
		this->offsetY = ENEMY_KOOPAS_DRAW_OFFSET_Y;
	}
	

	this->Add();

}

void KoopasIntro::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void KoopasIntro::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Destroy(this);

}

void KoopasIntro::Remove()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Remove(this);
}

void KoopasIntro::SetGrapper(PlayerIntro* player)
{
	this->mario = player;
}

void KoopasIntro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + ENEMY_KOOPAS_BBOX_OFFSET_X;
	top = y;
	right = left + width;
	bottom = top + height;
	//DebugOut(L"left: %f - top: %f - right: %f - bottom: %f\n", left, top, right, bottom);
}

void KoopasIntro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

		if (!immobilize)
		{
			if (type != eType::ENEMY_MOB_DIE)
				vx = direction * ENEMY_KOOPAS_MOVE_SPEED_X;
			else
				vx = direction * ENEMY_KOOPAS_WHIP_SPEED;
		}
			
		

		pointX = this->x + width / 2;
		pointY = this->y;
		//DebugOut(L"Grab: %d \n", beingGrab);
		if (type == eType::ENEMY)
		{
			if (beingGrab == true)
				CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY_MOB_DIE, eType::ENEMY_BULLET, eType::PLAYER_UNTOUCHABLE, eType::PLAYER, eType::ITEM });
			else if (rolling == false)
				CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY, eType::ENEMY_BULLET, eType::ENEMY_MOB_DIE, eType::PLAYER_UNTOUCHABLE, eType::ITEM });
			else if (rolling == true)
				CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY_MOB_DIE, eType::ENEMY_BULLET, eType::PLAYER_UNTOUCHABLE, eType::ITEM });
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
	}
	else if (rolling)
	{
		if (state == ENEMY_STATE_ROLL_SPECIAL)
			vx = direction * ENEMY_KOOPAS_ROLL_SPEED_DEFLECT;
		else
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


		if (ny != 0)
		{
			vy = 0;

			
			if (state == ENEMY_STATE_ROLL_SPECIAL)
				SetState(ENEMY_STATE_STOMP);
		}


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == eType::PLAYER)
			{
				float x, y;
				float vx, vy;
				e->obj->GetSpeed(vx, vy);

				if (e->ny > 0)
				{
					if (immobilize == false)
						SetState(ENEMY_STATE_STOMP);
					else
					{
						e->obj->GetPosition(x, y);
						float marioPosMiddle = x + e->obj->GetWidth() / 2;
						float enemyMiddle = this->x + width / 2;

						if (marioPosMiddle < enemyMiddle)
							direction = 1;
						else
							direction = -1;
						SetState(ENEMY_STATE_ROLLING);
					}
						
					e->obj->SetSpeed(vx, -MARIO_JUMP_DEFLECT_SPEED);
				}
				else if (e->ny < 0)
				{
					if (dynamic_cast<PlayerIntro*>(e->obj))
					{
						PlayerIntro* mario = static_cast<PlayerIntro*>(e->obj);
						mario->SetState(MARIO_STATE_HIT_BY_SHELL);

						this->vy = -ENEMY_KOOPAS_DEFLECT_Y;
						direction = -1;

						SetState(ENEMY_STATE_ROLL_SPECIAL);
					}
				}
				else
				{
					if (immobilize == false)
						e->obj->SetState(MARIO_STATE_HIT);
					else
					{
						if (mario->isGrappingPress())
						{
							mario->SetGrabObject(this);
							mario->SetState(MARIO_STATE_HOLD_SOMETHING);
							beingGrab = true;
						}
						else
						{
							mario->SetState(MARIO_STATE_KICK);
							mario->GetDirection(direction);
							SetState(ENEMY_STATE_KICK);
						}
					}
				}
			}
			else if (e->obj->GetType() == eType::ENEMY)
			{
				e->obj->SetState(ENEMY_STATE_HIT);
				e->obj->SetDirection(direction);

				if (beingGrab)
				{
					mario->GetDirection(direction);
					mario->SetState(MARIO_STATE_RELEASE_FULL);
					SetState(ENEMY_STATE_HIT);
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

	if (x < camPosX - ENTITY_SAFE_DELETE_RANGE || x > camPosX + ENTITY_SAFE_DELETE_RANGE ||
		y < camPosY - ENTITY_SAFE_DELETE_RANGE || y > camPosY + ENTITY_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}
}

void KoopasIntro::Render()
{
	int ani = -1;

	
	if (immobilize)
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
	else if (state == ENEMY_STATE_HIT || upSideDown)
	{
		if (direction > 0) ani = mobType + ENEMY_ANI_DIE_HIT_RIGHT;
		else ani = mobType + ENEMY_ANI_DIE_HIT_LEFT;
	}
	

	if (upSideDown)
		animation_set->Get(ani)->Render(x + offsetX, y + offsetY, 90.0f);
	else
		animation_set->Get(ani)->Render(x + offsetX, y + offsetY);
	//RenderBoundingBox();
}

void KoopasIntro::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();

	switch (state)
	{
		case ENEMY_STATE_STOMP:
			vx = 0;

			if (immobilize == false)
			{
				immobilize = true;
				rolling = false;

			}

			break;
		case ENEMY_STATE_ROLLING:
			immobilize = false;
			rolling = true;
			beingGrab = false;
			break;
		case ENEMY_STATE_ROLL_SPECIAL:
			immobilize = false;
			rolling = true;
			beingGrab = false;
			break;
		case ENEMY_STATE_KICK:
		{
			SetState(ENEMY_STATE_ROLLING);
			float x, y;
			float dx, dy;
			mario->GetPosition(x, y);
			mario->GetChange(dx, dy);
			dx = abs(dx);

			if (direction > 0)
				this->x = x + mario->GetWidth() + dx + 1.0f;
			else
				this->x = x - width - dx - 1.0f;
		}

			break;
		case ENEMY_STATE_BEING_GRAB:
			SetState(ENEMY_STATE_STOMP);

			mario->SetGrabObject(this);
			mario->SetState(MARIO_STATE_HOLD_SOMETHING);
			beingGrab = true;
			break;
		case ENEMY_STATE_RELEASE:
			beingGrab = false;
			break;
		case ENEMY_STATE_HIT:
			vy = -ENEMY_KOOPAS_DEFLECT;

			immobilize = false;
			this->type = eType::ENEMY_MOB_DIE;
			this->Remove();

			break;
	}
}
