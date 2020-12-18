#include "Plant.h"
#include "debug.h"

Plant::Plant(int placeX, int placeY, int mobType)
{
	this->x = placeX * STANDARD_SIZE + ENEMY_PLANT_OFFSET_X;
	this->y = placeY * STANDARD_SIZE + ENEMY_PLANT_OFFSET_Y;
	

	this->pause = false;
	this->width = ENEMY_PLANT_WIDTH;
	if (mobType == ENEMY_PIRANHA_GREEN || mobType == ENEMY_VENUS_GREEN)
		this->height = ENEMY_PLANT_HEIGHT_SHORT;
	else
		this->height = ENEMY_PLANT_HEIGHT_LONG;

	this->type = eType::ENEMY;
	this->state = ENEMY_STATE_MOVING;
	this->direction = 1;
	this->mobType = mobType;

	this->boundaryY_UP = round(this->y - this->height - ENEMY_PLANT_OFFSET_Y) + 1.0f;
	this->boundaryY_DOWN = this->y;

	this->showFace = false;
	this->moving = false;
	this->timePass = GetTickCount();

	this->Add();

}

void Plant::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Add(this);

	currentScene = scene;
}

void Plant::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Destroy(this);

}

void Plant::Remove()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Remove(this);
}

int Plant::GetDirection()
{
	if (marioPos_x < this->x)
	{
		if (marioPos_y < this->y)
			return 1;
		else
			return 2;
	}
	else
	{
		if (marioPos_y < this->y)
			return 3;
		else
			return 4;
	}

	return 0;
}

void Plant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + ENEMY_PLANT_BBOX_OFFSET_X;
	top = y;
	right = left + width;
	bottom = top + height;

}

void Plant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Check if outside camera
	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);

	if (x < camPosX - ENTITY_SAFE_DELETE_RANGE || x > camPosX + SCREEN_WIDTH + ENTITY_SAFE_DELETE_RANGE ||
		y < camPosY - ENTITY_SAFE_DELETE_RANGE || y > camPosY + SCREEN_HEIGHT + ENTITY_SAFE_DELETE_RANGE)
	{
		if (firstRun)
		{
			lastState = state;
			state = ENEMY_STATE_IDLE;

			timeShoot_dt = GetTickCount() - timeShoot;
			timePass_dt = GetTickCount() - timePass;

			firstRun = false;
		}
		
	}
	else
	{
		if (firstRun == false)
		{
			state = ENEMY_STATE_MOVING;
			firstRun = true;
		}
		
	}
		

	if (state == ENEMY_STATE_IDLE)
	{
		vx = 0;
		vy = 0;

		timeShoot = GetTickCount() - timeShoot_dt;
		timePass = GetTickCount() - timePass_dt;

		return;
	}

	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (PAUSE == false)
	{
		LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(currentScene);

		current->GetMarioPos(marioPos_x, marioPos_y);
		direction = GetDirection();

		if (moving == false)
		{
			if (GetTickCount() - timePass > ENEMY_PLANT_CYCLE_TIME)
			{
				moving = true;
			}
			else if (mobType >= ENEMY_VENUS_GREEN)
			{
				if (showFace)
				{
					if (GetTickCount() - timeShoot > ENEMY_PLANT_SHOOT_TIME)
					{
						SetState(ENEMY_STATE_SHOOT);
					}
				}	
			}
		}
		else
		{
			DWORD now = GetTickCount();

			if (showFace == false)
			{
				if (this->y <= boundaryY_UP)
				{
					this->y = boundaryY_UP;
					moving = false;
					showFace = true;
					timePass = now;
					timeShoot = now;
					vy = 0;
				}
				else
					vy = -ENEMY_PLANT_MOVE_SPEED;
			}
			else
			{
				if (this->y >= boundaryY_DOWN)
				{
					this->y = boundaryY_DOWN;
					moving = false;
					showFace = false;
					timePass = now;
					vy = 0;
				}
				else
					vy = ENEMY_PLANT_MOVE_SPEED;
			}
		}


		CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY, eType::ENEMY_MOB_DIE, eType::PLAYER_UNTOUCHABLE, eType::GROUP, 
			eType::ENEMY_BULLET, eType::ITEM});
	}
	else
	{
		vx = 0;
		vy = 0;
		return;
	}

	//DebugOut(L"moving: %d - %d\n", moving, showFace);
	//DebugOut(L"y - down: %f - %f\n", y, boundaryY_DOWN);
	//DebugOut(L"vy: %f \n", vy);
	

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

		x += dx;
		y += dy;


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == eType::PLAYER)
			{
				e->obj->SetState(MARIO_STATE_HIT);
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

void Plant::Render()
{
	if (firstRun == false)
		return;

	int ani = -1;

	if (mobType >= ENEMY_VENUS_GREEN)
	{
		if (moving)
		{
			if (direction == 1) ani = mobType + ENEMY_ANI_MOVE_LEFT_UP;
			else if (direction == 2) ani = mobType + ENEMY_ANI_MOVE_LEFT_DOWN;
			else if (direction == 3) ani = mobType + ENEMY_ANI_MOVE_RIGHT_UP;
			else if (direction == 4) ani = mobType + ENEMY_ANI_MOVE_RIGHT_DOWN;
		}
		else
		{
			if (direction == 1) ani = mobType + ENEMY_ANI_LEFT;
			else if (direction == 2) ani = mobType + ENEMY_ANI_LEFT_DOWN;
			else if (direction == 3) ani = mobType + ENEMY_ANI_RIGHT;
			else if (direction == 4) ani = mobType + ENEMY_ANI_RIGHT_DOWN;
		}
	}
	else
	{
		ani = mobType + ENEMY_ANI_LEFT;
	}

	animation_set->Get(ani)->Render(x + offsetX, y + offsetY);
	RenderBoundingBox();
}

void Plant::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();

	switch (state)
	{
	case ENEMY_STATE_SHOOT:
	{
		if (direction <= 2)
		{
			pointX = this->x;
			direct = -1;
		}
		else
		{
			pointX = this->x + width;
			direct = 1;
		}

		pointY = this->y + 8.0f;

		vectorX = marioPos_x - pointX;
		vectorY = marioPos_y - pointY;
		magnitude = float(sqrt(vectorX * vectorX + vectorY * vectorY));

		Fireball* bullet = new Fireball(pointX, pointY, direct, vectorX / magnitude, vectorY / magnitude);
	
		timeShoot = now;
	}
	break;

	case ENEMY_STATE_HIT:
	case ENEMY_STATE_HIT_TAIL:
	{
		vy = 0;

		moving = false;
		showFace = false;

		this->type = eType::ENEMY_MOB_DIE;
		this->Destroy();

		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
		current->FloatText(x, y);
	}

		break;
	}
}
