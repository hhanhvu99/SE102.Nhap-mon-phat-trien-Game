#include "Koopas.h"
#include "debug.h"

Koopas::Koopas(int placeX, int placeY, int mobType, bool hasWing)
{
	this->x = placeX * STANDARD_SIZE;
	this->y = placeY * STANDARD_SIZE - 1.0f;
	this->oldX = this->x;

	this->immobilize = false;
	this->rolling = false;
	this->comeBack = false;
	this->shaking = false;
	this->startShaking = false;
	this->beingGrab = false;
	this->upSideDown = false;

	this->width = ENEMY_KOOPAS_WIDTH;
	this->height = ENEMY_KOOPAS_HEIGHT;
	this->type = eType::ENEMY;

	this->state = ENEMY_STATE_MOVING;
	this->direction = -1;
	this->directionFly = -1;
	this->countTouch = 0;
	this->mobType = mobType;
	this->hasWing = hasWing;

	this->limitLeft = 99999999.9f;
	this->limitRight = 0.0f;

	this->pointUp = this->y + ENEMY_KOOPAS_FLY_BOUNDARY_UP;
	this->pointDown = this->y + ENEMY_KOOPAS_FLY_BOUNDARY_DOWN;

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

			shakeTime_dt = GetTickCount() - shakeTime;
			timeLeft_dt = GetTickCount() - timeLeft;

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

		shakeTime = GetTickCount() - shakeTime_dt;
		timeLeft = GetTickCount() - timeLeft_dt;

		return;
	}

	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (mario == NULL)
	{
		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
		mario = static_cast<Mario*>(current->GetMario());
	}

	// Simple fall down
	if (PAUSE == false)
	{
		if (hasWing && mobType == ENEMY_KOOPAS_RED)
		{
			vy += directionFly * ENEMY_KOOPAS_GRAVITY;
			vx = 0;

			if (this->y < pointUp)
				directionFly = 1;
			else if (this->y > pointDown)
				directionFly = -1;
				
			if (directionFly > 0)
			{
				if (vy > ENEMY_KOOPAS_MAX_FLY)
					vy = ENEMY_KOOPAS_MAX_FLY;
			}
			else
			{
				if (vy < -ENEMY_KOOPAS_MAX_FLY)
					vy = -ENEMY_KOOPAS_MAX_FLY;
			}

		}
		else
		{
			vy += ENEMY_KOOPAS_GRAVITY * dt;

			if (upSideDown)
			{
				if (immobilize)
				{
					if (abs(vx) < ENEMY_KOOPAS_THRESHOLD)
						vx = 0;
					else
						vx -= Global::Sign(vx) * ENEMY_KOOPAS_FRICTION * dt;
				}
			}
			else
			{
				if (!immobilize)
					vx = direction * ENEMY_KOOPAS_MOVE_SPEED_X;
			}

		}
		
		//DebugOut(L"Grab: %d \n", beingGrab);
		if (type == eType::ENEMY)
		{
			if (beingGrab == true)
				CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY_MOB_DIE, eType::ENEMY_BULLET, eType::PLAYER_UNTOUCHABLE, eType::PLAYER, eType::ITEM });
			else if (rolling == false)
				CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY_MOB_DIE, eType::ENEMY, eType::ENEMY_BULLET, eType::PLAYER_UNTOUCHABLE, eType::ITEM });
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

	if (immobilize)
	{
		if (GetTickCount() - timeLeft > ENEMY_KOOPAS_TIME_LEFT)
		{
			state = ENEMY_STATE_MOVING;

			immobilize = false;
			comeBack = false;
			shaking = false;
			startShaking = false;
			upSideDown = false;
			
			if (beingGrab)
			{
				beingGrab = false;
				mario->SetState(MARIO_STATE_HIT);
				SetState(ENEMY_STATE_MOVING);
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

	if (beingGrab)
	{
		vx = 0;
		vy = 0;
		dx = 0;
		dy = 0;
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

			if (hasWing && mobType != ENEMY_KOOPAS_RED)
			{
				if (ny < 0)
					vy = -ENEMY_KOOPAS_JUMP_SPEED;
			}
			else
			{
				if (ny < 0)
				{
					if (state == ENEMY_STATE_HIT_TAIL)
					{
						vy = -ENEMY_KOOPAS_BOUNDING;
						state = ENEMY_STATE_STOMP;
					}
				}
				
			}
				

		}

		bool hitEnemy = false;
		
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
						SetState(ENEMY_STATE_FLY_STOMP);
					}
					else
					{
						if (immobilize == false)
							SetState(ENEMY_STATE_STOMP);
						else
							SetState(ENEMY_STATE_ROLLING);
					}
					
					e->obj->SetSpeed(vx, -MARIO_JUMP_DEFLECT_SPEED);
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

				hitEnemy = true;
					
			}
			else if (rolling)
			{
				if (e->obj->GetType() == eType::QUESTION || e->obj->GetType() == eType::BRICK)
				{
					e->obj->SetState(BRICK_SHINY_STATE_HIT);

				}

				if (!hitEnemy)
				{
					if (nx > 0)
						direction = 1;
					else if (nx < 0)
						direction = -1;
				}
				
			}
			else if (nx != 0 && mobType == ENEMY_KOOPAS_GREEN)
			{
				if (nx < 0)
					direction = -1;
				else
					direction = 1;
			}
	
		}


		if (mobType == ENEMY_KOOPAS_RED && state == ENEMY_STATE_MOVING)
		{
			bool passCheck;
			bool onGroup = false;
			float thisLeft, thisTop, thisRight, thisBottom;		

			GetBoundingBox(thisLeft, thisTop, thisRight, thisBottom);
			thisLeft -= ENEMY_KOOPAS_CHECK_RANGE;
			thisTop -= ENEMY_KOOPAS_CHECK_RANGE;
			thisRight += ENEMY_KOOPAS_CHECK_RANGE;
			thisBottom += ENEMY_KOOPAS_CHECK_RANGE;

			if (abs(this->x - oldX) < ENEMY_KOOPAS_SIMILAR_GAP)
				countTouch += 1;
			oldX = this->x;


			for (auto object : *coObjects)
			{
				if (!onGroup)
					passCheck = false;
				float left, top, right, bottom;
				object->GetBoundingBox(left, top, right, bottom);

				if (onGroup == false)
				{
					if (object->GetType() == eType::BLOCK ||
						object->GetType() == eType::BRICK ||
						object->GetType() == eType::QUESTION)
					{
						if (left > thisLeft)
							if (top > thisTop)
								if (right < thisRight)
									if (bottom < thisBottom)
										passCheck = true;
					}
				}		
				if (object->GetType() == eType::GROUP ||
					object->GetType() == eType::PLATFORM)
				{
					for (auto eventCol : coEventsResult)
					{
						if (eventCol->obj == object)
						{
							limitLeft = left;
							limitRight = right;
							passCheck = true;
							onGroup = true;
							break;
						}
							
					}
						
				}
					
				//DebugOut(L"ongroup: %d\n", onGroup);

				if (passCheck == false)
					continue;


				if (object->GetState() == BRICK_SHINY_STATE_MOVING || object->GetState() == QUESTION_BLOCK_STATE_MOVING)
				{
					SetState(ENEMY_STATE_HIT);
					break;
				}

				if (countTouch < 3)
				{
					if (direction < 0)
					{
						if (!onGroup)
						{
							limitRight = 0.0f;
							if (left < limitLeft)
								limitLeft = left;
						}	
						if (this->x < limitLeft)
						{
							countTouch = 0;
							direction = 1;
						}
								
					}
					else
					{
						if (!onGroup)
						{
							limitLeft = 99999999.9f;
							if (right > limitRight)
								limitRight = right;
						}		
						if (this->x + width > limitRight)
						{
							countTouch = 0;
							direction = -1;
						}
					}

				}
				else
				{
					if (direction < 0)
						direction = 1;
					else
						direction = -1;
					countTouch = 0;
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

void Koopas::Render()
{
	if (firstRun == false)
		return;

	int ani = -1;

	if (hasWing)
	{
		if (state == ENEMY_STATE_MOVING)
		{
			if (direction > 0) ani = mobType + ENEMY_ANI_RIGHT_WING;
			else ani = mobType + ENEMY_ANI_LEFT_WING;
		}
		else if (state == ENEMY_STATE_HIT || upSideDown)
		{
			if (direction > 0) ani = mobType + ENEMY_ANI_DIE_HIT_RIGHT;
			else ani = mobType + ENEMY_ANI_DIE_HIT_LEFT;
		}
		
	}
	else 
	{
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
		else if (state == ENEMY_STATE_HIT || upSideDown)
		{
			if (direction > 0) ani = mobType + ENEMY_ANI_DIE_HIT_RIGHT;
			else ani = mobType + ENEMY_ANI_DIE_HIT_LEFT;
		}
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

	if (upSideDown)
		animation_set->Get(ani)->Render(x + offsetX, y + offsetY, 90.0f);
	else
		animation_set->Get(ani)->Render(x + offsetX, y + offsetY);
	//RenderBoundingBox();
}

void Koopas::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();

	switch (state)
	{
	case ENEMY_STATE_FLY_STOMP:
	{
		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
		current->FloatText(x, y);

		SetState(ENEMY_STATE_MOVING);
	}
		break;
	case ENEMY_STATE_STOMP:
	{
		vx = 0;

		timeLeft = now;
		comeBack = false;
		shaking = false;
		startShaking = false;

		if (immobilize == false)
		{
			immobilize = true;
			rolling = false;

		}

		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
		current->FloatText(x, y);
	}
		
		break;
	case ENEMY_STATE_ROLLING:
	{
		float x, y;
		mario->GetPosition(x, y);
		float marioPosMiddle = x + mario->GetWidth() / 2;
		float enemyMiddle = this->x + width / 2;

		if (marioPosMiddle < enemyMiddle)
			direction = 1;
		else
			direction = -1;

		immobilize = false;
		rolling = true;
		comeBack = false;
		shaking = false;
		startShaking = false;
		beingGrab = false;
	}

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
	case ENEMY_STATE_RELEASE:
		beingGrab = false;
		break;
	case ENEMY_STATE_HIT:
	{
		vy = -ENEMY_KOOPAS_DEFLECT_Y;

		immobilize = false;
		hasWing = false;
		this->type = eType::ENEMY_MOB_DIE;
		this->Remove();

		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
		current->FloatText(x, y);
	}	

		break;
	case ENEMY_STATE_HIT_TAIL:
	{
		vy = -ENEMY_KOOPAS_DEFLECT_TAIL;
		vx = direction * ENEMY_KOOPAS_WHIP_SPEED;

		timeLeft = now;
		comeBack = false;
		shaking = false;
		startShaking = false;
		hasWing = false;
		upSideDown = true;

		immobilize = true;
		rolling = false;

		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
		current->FloatText(x, y);
	}
			
		break;
	}
}
