#include "BrickShiny.h"
#include "debug.h"

BrickShiny::BrickShiny(float x, float y, LPSPRITE sprite) : ActiveBlock(x, y, sprite)
{
	this->x = oldX = x;
	this->y = oldY = y;
	this->hp = 1;
	this->countDestroy = 0;
	this->width = this->height = int(STANDARD_SIZE);

	this->moving = false;
	this->destroy = false;
	this->type = eType::BRICK;
	this->state = BRICK_SHINY_STATE_NORMAL;

	this->master = NULL;
}

/*
	Xóa object
*/
void BrickShiny::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	
	//Nếu object thuộc P-Block, xóa nó khỏi object holder của P-Block
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

	//Nếu gạch bị hit (đội gạch, quẩy đuôi, dính rùa,...)
	if (state == ACTIVE_BLOCK_STATE_HIT)
	{
		oldX = x;
		oldY = y;
		moving = true;
		startMoving = GetTickCount();
	}
	//Nếu bị bể, dừng lại 1 frame để xử lý đội gạch khi có rùa đứng trên
	if (destroy)
	{
		state = BRICK_SHINY_STATE_MOVING;

		if (countDestroy != 0)
			this->Destroy();
		++countDestroy;

		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		scene->activedBlock[indexY][indexX] = true;
		return;
	}

	//Di chuyển gạch khi đội, TH mario nhỏ
	float distanceX = oldX - x;
	float distanceY = oldY - y;

	if (distanceX + distanceY != 0 || moving == true)
		state = BRICK_SHINY_STATE_MOVING;
	else
		state = BRICK_SHINY_STATE_NORMAL;


	//DebugOut(L"State: %d \n", state);

	//Nếu di chuyển, đặt tốc độ
	if (moving)
		vy = -MOVING_SPEED;
	//Trong trạng thái di chuyển
	if (state == BRICK_SHINY_STATE_MOVING)
	{
		//Quá thời gian di chuyển, dừng lại
		if (GetTickCount() - startMoving > BLOCK_MOVING_TIME)
		{
			moving = false;
		}
		//Nếu dừng, cho gạch lùi từ từ về vị trí ban đầu
		if (moving == false)
		{
			//X khác
			if (abs(distanceX) >= BOUNDARY)
			{
				vx = Global::Sign(distanceX) * DEFLECT_SPEED;
			}
			else
			{
				vx = 0;
				x = oldX;
			}
			//Y khác
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
	//Ani thuộc phần ACTIVE_BLOCK
	int ani = BRICK_SHINY_ANI;
	animation_set->Get(ani)->Render(x, y);
	//RenderBoundingBox();
}

void BrickShiny::SetState(int state)
{
	this->state = state;

	switch (state)
	{
		//TH bị hit, nếu mario con và gạch không di chuyển thì tạo 4 viên gạch nhỏ
	case ACTIVE_BLOCK_STATE_HIT:
	{
		if (Global::GetInstance()->level != MARIO_LEVEL_SMALL && moving == false)
		{
			Rubbish* brick1 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED, -1);
			Rubbish* brick2 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED - 0.1f, -1);
			Rubbish* brick3 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED, 1);
			Rubbish* brick4 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED - 0.1f, 1);

			this->destroy = true;
		}	

	}
		break;
		//TH bị rùa hit
	case BRICK_SHINY_STATE_HIT:
	{
		Rubbish* brick1 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED, -1);
		Rubbish* brick2 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED - 0.1f, -1);
		Rubbish* brick3 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED, 1);
		Rubbish* brick4 = new Rubbish(x + RUBBISH_OFFSET_X, y + RUBBISH_OFFSET_Y, RUBBISH_SPEED_X, RUBBISH_SPEED_Y, RUBBISH_DEFLECT_SPEED - 0.1f, 1);

		this->destroy = true;
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
