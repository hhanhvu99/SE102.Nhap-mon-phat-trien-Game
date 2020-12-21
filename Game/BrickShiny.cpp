#include "BrickShiny.h"
#include "debug.h"

BrickShiny::BrickShiny(float x, float y, LPSPRITE sprite) : ActiveBlock(x, y, sprite)
{
	this->x = oldX = x;
	this->y = oldY = y;
	this->hp = 1;
	this->width = this->height = int(STANDARD_SIZE);

	this->moving = false;
	this->type = eType::BRICK;
	this->state = BRICK_SHINY_STATE_NORMAL;

	this->master = NULL;
}

void BrickShiny::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	
	if (master != NULL)
		static_cast<P_Block*>(master)->RemoveObject(this);

	current->Destroy(this);
}

void BrickShiny::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;

}

void BrickShiny::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	x += dx;
	y += dy;

	if (state == ACTIVE_BLOCK_STATE_HIT)
	{
		oldX = x;
		oldY = y;
		moving = true;
	}

	float distanceX = oldX - x;
	float distanceY = oldY - y;

	if (distanceX + distanceY != 0)
		state = BRICK_SHINY_STATE_MOVING;
	else
		state = BRICK_SHINY_STATE_NORMAL;


	//DebugOut(L"State: %d \n", state);

	if (moving)
		vy = -MOVING_SPEED;

	if (state == BRICK_SHINY_STATE_MOVING)
	{
		if (GetTickCount() - startMoving > BLOCK_MOVING_TIME)
		{
			moving = false;
		}
		
		if (moving == false)
		{
			if (abs(distanceX) >= BOUNDARY)
			{
				vx = Global::Sign(distanceX) * DEFLECT_SPEED;
			}
			else
			{
				vx = 0;
				x = oldX;
			}
			if (abs(distanceY) >= BOUNDARY)
			{
				vy = Global::Sign(distanceY) * DEFLECT_SPEED;
			}
			else
			{
				vy = 0;
				y = oldY;
			}
		}
		
	}

	//DebugOut(L"X: %f Y: %f OldX: %f OldY: %f\n", x, y, oldX, oldY);
}

void BrickShiny::Render()
{
	int ani = BRICK_SHINY_ANI;

	/*if (state == BRICK_SHINY_STATE_NORMAL)
	{
		ani = BRICK_SHINY_ANI_1;
	}*/

	animation_set->Get(ani)->Render(x, y);
	//RenderBoundingBox();
}

void BrickShiny::SetState(int state)
{
	this->state = state;

	switch (state)
	{
	case ACTIVE_BLOCK_STATE_HIT:
	{
		Rubbish* brick1 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED, -1);
		Rubbish* brick2 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED - 0.1f, -1);
		Rubbish* brick3 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED, 1);
		Rubbish* brick4 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED - 0.1f, 1);

		this->Destroy();
	}
		break;

	default:
		break;
	}

}

BrickShiny::~BrickShiny()
{
	master = NULL;
}
