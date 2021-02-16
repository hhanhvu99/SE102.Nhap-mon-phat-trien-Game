#include "QuestionBlock.h"
#include "debug.h"

QuestionBlock::QuestionBlock(float x, float y, LPSPRITE sprite) : ActiveBlock(x, y, sprite)
{
	this->x = oldX = x;
	this->y = oldY = y;
	this->width = this->height = int(STANDARD_SIZE);

	this->hp = 1;
	this->hit = false;
	this->moving = false;
	this->type = eType::QUESTION;
	this->state = QUESTION_BLOCK_STATE_NORMAL;

}

void QuestionBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void QuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	x += dx;
	y += dy;
	
	//Nếu bị hit lần đầu (đội gạch, dính rùa, quật đuôi,...)
	if (state == ACTIVE_BLOCK_STATE_HIT && hit == false)
	{
		oldX = x;
		oldY = y;
		hit = true;
		moving = true;
		this->hp -= 1;
		startMoving = GetTickCount();

		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		scene->activedBlock[indexY][indexX] = true;
	}

	float distanceX = oldX - x;
	float distanceY = oldY - y;

	//Di chuyển khi bị đội
	if (distanceX + distanceY != 0 || moving == true)
		state = QUESTION_BLOCK_STATE_MOVING;
	else
		state = QUESTION_BLOCK_STATE_NORMAL;

	if (moving)
		vy = -MOVING_SPEED;

	if (state == QUESTION_BLOCK_STATE_MOVING)
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
			//Với TH y thay đổi, khi kết thúc sẽ spawn một item nằm trong đó
			if (abs(distanceY) >= BOUNDARY)
			{
				vy = Global::Sign(distanceY) * DEFLECT_SPEED;
			}
			else
			{
				vy = 0;
				y = oldY;
				int currentItemCell = -1;

				//Spawn item
				if (item)
				{
					//Là Leaf
					if (item->getItemType() == ITEM_SUPER_LEAF)
					{	
						//Nếu mario level khác nhỏ, spawn leaf
						if (Global::GetInstance()->level > MARIO_LEVEL_SMALL)
							item->SetState(ITEM_STATE_SHOW);
						//Nếu mario level nhỏ, spawn cục nấm
						else
						{
							item->Replace();
							item = new Mushroom(x, y, ITEM_MUSHROOM_RED);
							item->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
							item->SetState(ITEM_STATE_SHOW);
						}
					}
					//Show item
					else
					{
						item->SetState(ITEM_STATE_SHOW);

					}
					//HP của block lớn hơn 0, coin nhảy lên
					if (hp > 0)
					{
						currentItemCell = item->GetCurrentCell();
						item = new Coin(x, y, ITEM_COIN, false);
						item->SetDrawOrder(BLOCK_DRAW_ORDER);
						item->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
						static_cast<LPTESTSCENE>(SceneManager::GetInstance()->GetCurrentScene())->AddToCell(currentItemCell, item);
						hit = false;
					}
					else
						item = NULL;
				}
			}
		}
	}

	//DebugOut(L"State Update: %d \n", state);
}

void QuestionBlock::Render()
{
	//Ani thuộc ACTIVE_BLOCK
	//Nếu hp lớn hơn 0
	if (hp > 0)
	{
		int ani = -1;
		if (option == 0)
			ani = QUESTION_BLOCK_ANI;
		else
			ani = BRICK_SHINY_ANI;
		animation_set->Get(ani)->Render(x, y);
	}
	//Nếu hp = 0
	else
	{
		sprite->Draw(x, y);
	}

	//RenderBoundingBox();

}

void QuestionBlock::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	current->Destroy(this);
}

void QuestionBlock::SetState(int state)
{
	this->state = state;

	//Nếu block đã active xong
	if (state == QUESTION_BLOCK_STATE_DONE)
	{
		hit = true;
		hp = 0;
	}
}

