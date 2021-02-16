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

	this->draw_order = BULLET_DRAW_ORDER;
	this->bulletType = BULLET_FIREBALL_TYPE_NORMAL;
	this->type = eType::MARIO_BULLET;
	this->state = BULLET_STATE_MOVING;
	this->animation_set = AnimationManager::GetInstance()->Get(BULLET);
	Global::GetInstance()->numberOfFireBall += 1;

	this->Add();
}

Fireball::Fireball(float x, float y, int direction, float unitVectorX, float unitVectorY)
{
	this->x = x;
	this->y = y;
	this->unitVectorX = unitVectorX;
	this->unitVectorY = unitVectorY;
	this->offsetX = BULLET_FIREBALL_DRAW_OFFSET_X;
	this->offsetY = BULLET_FIREBALL_DRAW_OFFSET_Y;
	this->width = BULLET_FIREBALL_WIDTH;
	this->height = BULLET_FIREBALL_HEIGHT;
	this->direction = direction;

	this->draw_order = BULLET_DRAW_ORDER;
	this->bulletType = BULLET_FIREBALL_TYPE_NORMAL;
	this->type = eType::ENEMY_BULLET;

	this->state = BULLET_STATE_MOVING;
	this->animation_set = AnimationManager::GetInstance()->Get(BULLET);

	this->Add();
}

void Fireball::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Add(this);

	//Lấy mario
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
	if (PAUSE == false)
	{
		//Là đạn của mario
		if (type == eType::MARIO_BULLET)
		{
			//Di chuyển thep đường vòng cung
			vy += BULLET_FIREBALL_GRAVITY * dt;
			vx = direction * BULLET_FIREBALL_SPEED_X;

			if (state != BULLET_STATE_HIT)
				CalcPotentialCollisions(coObjects, coEvents, { eType::PLAYER , eType::ENEMY_MOB_DIE, eType::ENEMY_BULLET, eType::ITEM });
		}
		//Không là đạn của mario
		else
		{
			//Đạn di chuyển theo đường thẳng chạy theo vị trí của mario
			vy = unitVectorY * BULLET_FIREBALL_SPEED_ENEMY;
			vx = unitVectorX * BULLET_FIREBALL_SPEED_ENEMY;
			if (state != BULLET_STATE_HIT)
				CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY, eType::ENEMY_BULLET, eType::MARIO_BULLET, eType::ENEMY_MOB_DIE, eType::P_BLOCK,
					eType::BLOCK, eType::GROUP, eType::BRICK, eType::QUESTION, eType::PLAYER_UNTOUCHABLE, eType::PLATFORM , eType::ITEM, eType::GROUP_MOVING});
		}
		
	}
	else
	{
		vx = 0;
		vy = 0;
		return;
	}
		
	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);
	//Đạn đứng yên
	if (state == BULLET_STATE_IDLE)
	{
		vx = 0;
		vy = 0;
	}
	//Đạn hit
	else if (state == BULLET_STATE_HIT)
	{
		vx = 0;
		vy = 0;

		//Đợi hết animation thì xóa
		if (GetTickCount() - timeLeft > BULLET_FIREBALL_TIME_LEFT)
		{
			this->Destroy();
			return;
		}
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//DebugOut(L"vx: %f -- vy: %f\n", vx, vy);
		//DebugOut(L"nx: %f -- ny: %f", nx, ny);

		//Di chuyển
		x += dx;

		//Nếu là đạn mario
		if (type == eType::MARIO_BULLET)
		{
			this->y += min_ty * dy + ny * 0.4f;

			//Nếu đạn hit, tương ứng chạm vào 2 bên hoặc chạm bên trên
			if (nx != 0 || ny > 0)
			{
				SetState(BULLET_STATE_HIT);
			}
			//Nếu đụng dưới
			else if (ny < 0)
			{
				vy = -BULLET_FIREBALL_DEFLECT_SPEED;
			}
		}
		
		//Xét các TH va chạm
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Là enemy
			if (e->obj->GetType() == eType::ENEMY)
			{
				e->obj->SetState(ENEMY_STATE_HIT);
				e->obj->SetDirection(direction);
				SetState(BULLET_STATE_HIT);
			}
			//Là player
			else if (e->obj->GetType() == eType::PLAYER)
			{
				e->obj->SetState(MARIO_STATE_HIT);
				this->Destroy();
			}
			
		}

	}

	//DebugOut(L"x: %f -- y:%f\n", x, y);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
		coEvents[i] = NULL;
	}

	//Ra khỏi camera thì xóa
	if (x < camPosX - BULLET_SAFE_DELETE_RANGE || x > camPosX + SCREEN_WIDTH + BULLET_SAFE_DELETE_RANGE ||
		y < camPosY - BULLET_SAFE_DELETE_RANGE || y > camPosY + SCREEN_HEIGHT + BULLET_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}
}

void Fireball::Render()
{
	//Ani thuộc BULLET
	int ani = -1;

	//Nếu chưa bị hit
	if (state != BULLET_STATE_HIT)
	{
		if (direction > 0) ani = bulletType + BULLET_ANI_RIGHT;
		else ani = bulletType + BULLET_ANI_LEFT;
	}
	//Bị hit, chạy animation
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
		break;
	case BULLET_STATE_MOVING:
		break;
	case BULLET_STATE_HIT:
		timeLeft = now;
		bulletType = BULLET_EFFECT_POP;
		break;
	}
}

Fireball::~Fireball()
{
	if (type == eType::MARIO_BULLET)
		Global::GetInstance()->numberOfFireBall -= 1;

	animation_set = NULL;
	sprite = NULL;

}
