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
	this->rollSpeed = ENEMY_MENU_KOOPAS_ROLL_SPEED_X;

	this->width = ENEMY_KOOPAS_WIDTH;
	this->height = ENEMY_KOOPAS_HEIGHT;
	this->type = eType::ENEMY;

	this->direction = 1;
	this->mobType = mobType;
	this->special = false;

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
		vy += ENEMY_MENU_KOOPAS_GRAVITY * dt;

		//Nếu rùa không bất động
		if (!immobilize)
		{
			//Nếu rùa không chết
			if (type != eType::ENEMY_MOB_DIE)
			{
				if (special)
					vx = direction * ENEMY_MENU_KOOPAS_MOVE_SPEED_FAST;
				else
					vx = direction * ENEMY_MENU_KOOPAS_MOVE_SPEED_X;
			}
			//Nếu chết
			else
			{
				vx = direction * ENEMY_KOOPAS_WHIP_SPEED;
			}
				
		}
			
		//DebugOut(L"Grab: %d \n", beingGrab);
		//Nếu enemy chưa chết
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
	//Đứng yên
	if (state == ENEMY_STATE_IDLE)
	{
		vx = 0;
		vy = 0;
	}
	//Bất động
	else if (immobilize)
	{
		vx = 0;
	}
	//Xoay
	else if (rolling)
	{
		if (state == ENEMY_STATE_ROLL_SPECIAL)
			vx = direction * ENEMY_MENU_KOOPAS_ROLL_SPEED_DEFLECT;
		else
			vx = direction * rollSpeed;
	}
	//Bị cầm
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		//DebugOut(L"vx: %f -- vy: %f\n", vx, vy);
		//DebugOut(L"nx: %f -- ny: %f -- direction: %d\n", nx, ny, direction);

		this->x += min_tx * dx + nx * 0.4f;
		this->y += min_ty * dy + ny * 0.4f;

		//Nếu chạm cái gì đó, tốc độ rơi bằng 0
		if (ny != 0)
		{
			vy = 0;

			
			if (state == ENEMY_STATE_ROLL_SPECIAL)
				SetState(ENEMY_STATE_STOMP);
		}

		//DebugOut(L"size: %d\n", coEventsResult.size());

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Là mario
			if (e->obj->GetType() == eType::PLAYER)
			{
				float x, y;
				float vx, vy;
				e->obj->GetSpeed(vx, vy);

				//Nếu bị đạp
				if (e->ny > 0)
				{
					//TH không bất động, chuyển sang trạng thái bất động
					if (immobilize == false)
						SetState(ENEMY_STATE_STOMP);
					//TH bất động, dựa vào vị trí của mario và vị trí của rùa để xác định hướng đi
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
				//TH đụng trúng gì ở dưới, chỉ xảy ra trong intro
				else if (e->ny < 0)
				{
					if (dynamic_cast<PlayerIntro*>(e->obj))
					{
						PlayerIntro* mario = static_cast<PlayerIntro*>(e->obj);
						mario->SetState(MARIO_MENU_STATE_HIT_BY_SHELL);

						this->vy = -ENEMY_MENU_KOOPAS_DEFLECT_Y;
						direction = -1;

						SetState(ENEMY_STATE_ROLL_SPECIAL);
					}
				}
				//TH còn lại
				else
				{
					//Nếu không bất động, mario bị hit
					if (immobilize == false)
						e->obj->SetState(MARIO_MENU_STATE_HIT);
					//Nếu bất động, nghĩa là mario đá mai rùa hoặc cầm mai rùa
					else
					{
						//Cầm mai rùa
						if (mario->isGrappingPress())
						{
							mario->SetGrabObject(this);
							mario->SetState(MARIO_STATE_HOLD_SOMETHING);
							beingGrab = true;
						}
						//Đá mai rùa
						else
						{
							mario->SetState(MARIO_MENU_STATE_KICK);
							mario->GetDirection(direction);
							SetState(ENEMY_STATE_KICK);
						}
					}
				}
			}
			//Là enemy
			else if (e->obj->GetType() == eType::ENEMY)
			{
				//Hit enemy
				e->obj->SetState(ENEMY_STATE_HIT);
				e->obj->SetDirection(direction);

				//Nếu bị cầm thì rùa này bị hit
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

}

void KoopasIntro::Render()
{
	//Ani thuộc ENEMY_MOB
	int ani = -1;

	//Bất động
	if (immobilize)
	{
		ani = mobType + ENEMY_ANI_IMMOBILIZE;
	}
	//Xoay
	else if (rolling)
	{
		ani = mobType + ENEMY_ANI_ROLLING;
	}
	//Di chuyển
	else if (state == ENEMY_STATE_MOVING)
	{
		if (direction > 0) ani = mobType + ENEMY_ANI_RIGHT;
		else ani = mobType + ENEMY_ANI_LEFT;
	}
	//Đuôi quật trúng hoặc bị hit
	else if (state == ENEMY_STATE_HIT || upSideDown)
	{
		if (direction > 0) ani = mobType + ENEMY_ANI_DIE_HIT_RIGHT;
		else ani = mobType + ENEMY_ANI_DIE_HIT_LEFT;
	}
	
	//Mai rùa lộn ngược
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
		//Bị đạp
		case ENEMY_STATE_STOMP:
			vx = 0;

			if (immobilize == false)
			{
				immobilize = true;
				rolling = false;

			}

			break;
		//Xoay
		case ENEMY_STATE_ROLLING:
			immobilize = false;
			rolling = true;
			beingGrab = false;
			break;
		//Xoay trong intro
		case ENEMY_STATE_ROLL_SPECIAL:
			immobilize = false;
			rolling = true;
			beingGrab = false;
			break;
		//Bị đá
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
		//Bị cầm
		case ENEMY_STATE_BEING_GRAB:
			SetState(ENEMY_STATE_STOMP);

			mario->SetGrabObject(this);
			mario->SetState(MARIO_STATE_HOLD_SOMETHING);
			beingGrab = true;
			break;
		//Thả ra
		case ENEMY_STATE_RELEASE:
			beingGrab = false;
			break;
		//Bị giết
		case ENEMY_STATE_HIT:
			vy = -ENEMY_MENU_KOOPAS_DEFLECT;

			immobilize = false;
			this->type = eType::ENEMY_MOB_DIE;
			this->Remove();

			break;
	}
}
