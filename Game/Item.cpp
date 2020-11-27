#include "Item.h"
#include "debug.h"

Item::Item(float x, float y, int itemType)
{
	this->x = x;
	this->y = y;
	this->width = this->height = STANDARD_SIZE;
	this->boundaryUp = this->y - STANDARD_SIZE;
	this->direction = 1;

	this->type = eType::ENEMY_MOB_DIE;
	this->itemType = itemType;
	this->state = ITEM_STATE_IDLE;

	this->showUp = false;
	this->stop = true;
	this->leafBreak = false;
	this->leafBreak_Y = false;

	this->Add();
}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void Item::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void Item::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = dynamic_cast<LPTESTSCENE>(scene);

	current->Destroy(this);

}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (state == ITEM_STATE_IDLE)
	{
		vx = 0;
		vy = 0;
		return;
	}

	// Simple fall down
	if (PAUSE == false)
	{
	
		if (itemType != ITEM_SUPER_LEAF)
		{	
			if (showUp == false)
			{
				vy += ITEM_GRAVITY * dt;
			}

			CalcPotentialCollisions(coObjects, coEvents, { eType::ITEM, eType::ENEMY, eType::ENEMY_BULLET,
					eType::PLAYER_UNTOUCHABLE, eType::ENEMY_MOB_DIE });
		}
			

		if (state == ITEM_STATE_MOVING)
		{
			vy += ITEM_LEAF_SPEED_Y * dt;

			if (itemType != ITEM_SUPER_LEAF)
				vx = direction * ITEM_MOVE_SPEED_X;
			else
			{
				if (stop)
				{
					direction = -direction;

					vx = 0;
					vy = 0;

					stop = false;
					leafBreak = false;
				}
				else
				{
					if (direction > 0)
					{
						if (leafBreak)
						{
							vx -= ITEM_FRICTION * dt;

							if (vx <= 0)
								stop = true;
						}
						else if (vx >= ITEM_LEAF_BOUNDARY)
						{
							leafBreak = true;
						}
						else
							vx += ITEM_LEAF_SPEED_X * dt;
					}
					else
					{
						if (leafBreak)
						{
							vx += ITEM_FRICTION * dt;

							if (vx >= 0)
								stop = true;
						}
						else if (vx <= -ITEM_LEAF_BOUNDARY)
						{
							leafBreak = true;
						}
						else
							vx += -ITEM_LEAF_SPEED_X * dt;
					}

				}
			}
		}
		else if (state == ITEM_STATE_SHOW)
		{
			if (itemType != ITEM_SUPER_LEAF)
			{
				if (this->y <= boundaryUp)
				{
					showUp = false;
					state = ITEM_STATE_MOVING;
					type = eType::ITEM;
				}
				else
				{
					vy = -ITEM_MOVE_SPEED_Y;
				}
			}
			else 
			{
				if (vy > 0)
				{
					showUp = false;
					state = ITEM_STATE_MOVING;
					type = eType::ITEM;
				}
				else
					vy += ITEM_GRAVITY * dt;
			}
		
		}
		
	}
	else
	{
		vx = 0;
		vy = 0;
		return;
	}

	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);

	// No collision occured, proceed normally
	//DebugOut(L"Size: %d \n", coEvents.size());
	//DebugOut(L"y: %f\n", this->y);

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		this->x += min_tx * dx + nx * 0.4f;
		this->y += min_ty * dy + ny * 0.4f;
	
		if (ny != 0)
		{
			vy = 0;

			if (ny < 0)
				if (itemType == ITEM_SUPER_STAR)
					vy -= ITEM_MOVE_SPEED_Y;
		}

		if (nx > 0)
			direction = 1;
		else if (nx < 0)
			direction = -1;

		//DebugOut(L"vx: %f -- vy: %f\n", vx, vy);
		//DebugOut(L"nx: %f -- ny: %f", nx, ny);
		

	}

	//DebugOut(L"x: %f -- y:%f\n", x, y);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
		coEvents[i] = NULL;
	}

	if (x < camPosX - ITEM_SAFE_DELETE_RANGE || x > camPosX + ITEM_SAFE_DELETE_RANGE ||
		y < camPosY - ITEM_SAFE_DELETE_RANGE || y > camPosY + ITEM_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}
}

void Item::Render()
{
	if (state != ITEM_STATE_IDLE)
	{
		int ani = -1;

		if (itemType != ITEM_SUPER_LEAF)
		{
			ani = itemType;
		}
		else
		{
			if (direction > 0) ani = itemType + ITEM_ANI_RIGHT;
			else ani = itemType + ITEM_ANI_LEFT;

		}

		animation_set->Get(ani)->Render(x, y);
		//RenderBoundingBox();
	}

}

void Item::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case ITEM_STATE_SHOW:
		showUp = true;

		if (itemType == ITEM_SUPER_LEAF)
			vy = -ITEM_JUMP_SPEED;

		break;
	case ITEM_STATE_HIT:
		this->Destroy();
		break;
	default:
		break;
	}
}