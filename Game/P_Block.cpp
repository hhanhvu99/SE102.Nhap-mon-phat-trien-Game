#include "P_Block.h"
#include "debug.h"

P_Block::P_Block(float x, float y) : GameObject()
{
	this->x = this->currentX = x;
	this->y = this->currentY = y;
	this->width = STANDARD_SIZE;
	this->height = STANDARD_SIZE;

	this->state = P_BLOCK_STATE_IDLE;
	this->listOfObject.clear();
	this->type = eType::P_BLOCK;

	this->sprite = SpriteManager::GetInstance()->Get(QUESTION_BLOCK_ANI_HIT);
	this->p_Block = SpriteManager::GetInstance()->Get(P_BLOCK_ANI);
	this->stomp_Block = SpriteManager::GetInstance()->Get(P_BLOCK_ANI_STOMP);

}

void P_Block::AddObject(LPGAMEOBJECT object)
{
	listOfObject.push_back(object);

}

void P_Block::ChangeToCoin()
{
	LPTESTSCENE tempScene = static_cast<TestScene*>(SceneManager::GetInstance()->GetCurrentScene());
	LPGAMEOBJECT tempCoin;
	int indexX, indexY;
	float posX, posY;
	vector<LPGAMEOBJECT> tempVector(listOfObject);

	listOfObject.clear();

	for (auto object : tempVector)
	{
		object->GetIndex(indexX, indexY);
		object->GetPosition(posX, posY);

		tempCoin = new Coin(posX, posY, ITEM_COIN, true);
		tempCoin->SetDrawOrder(BLOCK_DRAW_ORDER);
		tempCoin->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
		listOfObject.push_back(tempCoin);

		tempScene->Destroy(object);
	}

}

void P_Block::RemoveObject(LPGAMEOBJECT objectToDelete)
{
	listOfObject.erase(std::remove(listOfObject.begin(), listOfObject.end(), objectToDelete), listOfObject.end());

}

void P_Block::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (switchToP)
	{
		left = x;
		top = y - STANDARD_SIZE;
		right = left + width;
		bottom = top + height + STANDARD_SIZE;
	}
	else
	{
		left = x;
		top = y;
		right = left + width;
		bottom = top + height;
	}
}

void P_Block::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	currentX += dx;
	currentY += dy;

	if (state == ACTIVE_BLOCK_STATE_HIT && hit == false)
	{
		oldX = currentX;
		oldY = currentY;

		switchToP = true;
		hit = true;

		moving = true;

		startMoving = GetTickCount();
	}
	else if (state == P_BLOCK_STATE_STOMP)
	{
		stomp = true;
		switchToP = false;
	}

	if (switchToP)
	{
		float distanceX = oldX - currentX;
		float distanceY = oldY - currentY;

		if (distanceX + distanceY != 0)
			state = P_BLOCK_STATE_MOVING;
		else
			state = P_BLOCK_STATE_NORMAL;
		
			
		//DebugOut(L"State: %d \n", state);

		if (moving)
			vy = -MOVING_SPEED;

		if (state == P_BLOCK_STATE_MOVING)
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
					currentX = oldX;
				}
				if (abs(distanceY) >= BOUNDARY)
				{
					vy = Global::Sign(distanceY) * DEFLECT_SPEED;
				}
				else
				{
					vy = 0;
					currentY = oldY;
				}
			}

		}
	}
	
}

void P_Block::Render()
{
	if (stomp)
	{
		sprite->Draw(x, y);
		stomp_Block->Draw(x, y - height);
	}
	else if (switchToP == false)
	{
		animation_set->Get(BRICK_SHINY_ANI)->Render(x, y);
	}
	else if (switchToP)
	{
		p_Block->Draw(x, y - height);
		sprite->Draw(currentX, currentY);
	}
}

void P_Block::SetState(int state)
{
	GameObject::SetState(state);

	if (state == P_BLOCK_STATE_STOMP)
	{
		ChangeToCoin();
	}

}
