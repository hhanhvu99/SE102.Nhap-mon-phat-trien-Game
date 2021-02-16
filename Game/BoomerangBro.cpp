#include "BoomerangBro.h"
#include "debug.h"

BoomerangBro::BoomerangBro(int placeX, int placeY, int mobType)
{
	this->x = placeX * STANDARD_SIZE;
	this->y = placeY * STANDARD_SIZE - (ENEMY_BRO_HEIGHT - STANDARD_SIZE) - 1.0f;

	this->touchGround = true;
	this->steady = true;
	this->width = ENEMY_BRO_WIDTH;
	this->height = ENEMY_BRO_HEIGHT;
	this->type = eType::ENEMY;

	this->state = ENEMY_STATE_MOVING;
	this->direction = -1;
	this->mobType = mobType;
	this->mario = NULL;

	this->timeLeft = GetTickCount();
	this->timeJump = GetTickCount();

	this->Add();

}

void BoomerangBro::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void BoomerangBro::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Destroy(this);

}

void BoomerangBro::Remove()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Remove(this);
}

void BoomerangBro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + ENEMY_BRO_WIDTH;
	bottom = top + ENEMY_BRO_HEIGHT;

}

void BoomerangBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Check outside camera
	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);
	if (x < camPosX - ENTITY_SAFE_DELETE_RANGE || x > camPosX + SCREEN_WIDTH + ENTITY_SAFE_DELETE_RANGE ||
		y < camPosY - ENTITY_SAFE_DELETE_RANGE || y > camPosY + SCREEN_HEIGHT + ENTITY_SAFE_DELETE_RANGE)
	{
		if (y > Global::GetInstance()->screenHeight)
		{
			Destroy();
			return;
		}
		if (firstRun)
		{
			lastState = state;
			state = ENEMY_STATE_IDLE;

			timeLeft_dt = GetTickCount() - timeLeft;
			timeJump_dt = GetTickCount() - timeJump;
			timeThrow_dt = GetTickCount() - timeThrow_dt;

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

	if (mario == NULL)
	{
		mario = static_cast<TestScene*>(SceneManager::GetInstance()->GetCurrentScene())->GetMario();
	}

	if (state == ENEMY_STATE_IDLE)
	{
		vx = 0;
		vy = 0;

		timeLeft = GetTickCount() - timeLeft_dt;
		timeJump = GetTickCount() - timeJump_dt;
		timeThrow = GetTickCount() - timeThrow_dt;

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
		//Nếu chưa chết
		if (type != eType::ENEMY_MOB_DIE)
		{
			float tempX, tempY;
			mario->GetPosition(tempX, tempY);

			//Nằm trong vùng ném
			if (abs(this->x - tempX) < ENEMY_BRO_THROW_RANGE)
			{
				//Chuyển hướng qua lại, mặt hướng về mario
				if (GetTickCount() - timeLeft > ENEMY_BRO_TIME_SWITCH)
				{
					direction = -direction;
					timeLeft = GetTickCount();
				}
				//Nhảy lên
				if (GetTickCount() - timeJump > ENEMY_BRO_TIME_JUMP)
				{
					vy = -ENEMY_BRO_JUMP;
					timeJump = GetTickCount();
				}
				//Sẵn sàng ném
				if (steady)
				{
					if (GetTickCount() - timeThrow > ENEMY_BRO_TIME_THROW_READY)
					{
						steady = false;
						timeThrow = GetTickCount();

						if (direction > 0)
							Boomerang* boomerang = new Boomerang(x + width / 2, y, facing, this);
						else
							Boomerang* boomerang = new Boomerang(x, y, facing, this);

					}
				}
				//Đợi sẵn sàng
				else
				{
					if (GetTickCount() - timeThrow > ENEMY_BRO_TIME_THROW_WAIT)
					{
						steady = true;
						timeThrow = GetTickCount();
					}
				}
				
				//Vị trí giữa mario với enemy này, facing = 1 quay mặt bên phải, facing = -1 quay mặt bên trái
				if (this->x < tempX)
					facing = 1;
				else
					facing = -1;
			}
			//Nằm ngoài vùng ném, tự động di chuyển tới mario
			else
			{
				direction = -Global::GetInstance()->Sign(this->x - tempX);
				facing = direction;
				timeLeft = timeJump = timeThrow = GetTickCount();
			}
		}
		
		vy += ENEMY_BRO_GRAVITY * dt;
		vx = direction * ENEMY_BRO_MOVE_SPEED_X;

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
		float nx, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		//DebugOut(L"vx: %f -- vy: %f\n", vx, vy);
		//DebugOut(L"nx: %f -- ny: %f\n", nx, ny);

		this->x += min_tx * dx + nx * 0.4f;
		this->y += min_ty * dy + ny * 0.4f;


		if (ny != 0)
		{
			vy = 0;

			if (ny < 0)
				touchGround = true;

		}


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == eType::PLAYER)
			{
				float vx, vy;
				e->obj->GetSpeed(vx, vy);

				//Đạp trúng đầu
				if (e->ny > 0)
				{
					SetState(ENEMY_STATE_STOMP);
					e->obj->SetSpeed(vx, -MARIO_JUMP_DEFLECT_SPEED);

				}
				//Đụng trúng mario
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

void BoomerangBro::Render()
{
	//Ani thuộc ENEMY_MOB
	if (firstRun == false)
		return;

	int ani = -1;

	if (state == ENEMY_STATE_MOVING)
	{
		if (steady)
		{
			if (facing > 0) ani = mobType + ENEMY_ANI_RIGHT_STEADY;
			else ani = mobType + ENEMY_ANI_LEFT_STEADY;
		}
		else
		{
			if (facing > 0) ani = mobType + ENEMY_ANI_RIGHT_THROW;
			else ani = mobType + ENEMY_ANI_LEFT_THROW;
		}
		
	}
	else if (state == ENEMY_STATE_STOMP ||state == ENEMY_STATE_HIT || state == ENEMY_STATE_HIT_TAIL)
	{
		if (direction > 0) ani = mobType + ENEMY_ANI_DIE_HIT_RIGHT;
		else ani = mobType + ENEMY_ANI_DIE_HIT_LEFT;
	}
	


	animation_set->Get(ani)->Render(x, y);
	//RenderBoundingBox();
}

void BoomerangBro::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();

	switch (state)
	{
		//Bị đạp
	case ENEMY_STATE_STOMP:
	{
		this->type = eType::ENEMY_MOB_DIE;
		this->Remove();

		if (CHOOSE != 2)
		{
			LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
			LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
			current->FloatText(x, y);
		}

	}

	break;
		//Bị hit
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
