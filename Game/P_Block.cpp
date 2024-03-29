﻿#include "P_Block.h"
#include "debug.h"

P_Block::P_Block(float x, float y) : GameObject()
{
	this->x = this->currentX = x;
	this->y = this->currentY = y;
	this->width = int(STANDARD_SIZE);
	this->height = int(STANDARD_SIZE);

	this->state = P_BLOCK_STATE_IDLE;
	this->listOfObject.clear();
	this->type = eType::P_BLOCK;

	this->sprite = SpriteManager::GetInstance()->Get(QUESTION_BLOCK_ANI_HIT);
	this->p_Block = SpriteManager::GetInstance()->Get(P_BLOCK_ANI);
	this->stomp_Block = SpriteManager::GetInstance()->Get(P_BLOCK_ANI_STOMP);

}

/*
	Thêm object vào P-Block holder
*/
void P_Block::AddObject(LPGAMEOBJECT object)
{
	listOfObject.push_back(object);

}

/*
	Chuyển các object về coin
*/
void P_Block::ChangeToCoin()
{
	LPTESTSCENE tempScene = static_cast<TestScene*>(SceneManager::GetInstance()->GetCurrentScene());
	LPGAMEOBJECT tempCoin;
	int indexX, indexY;
	float posX, posY;
	//Copy từ object holder
	vector<LPGAMEOBJECT> tempVector(listOfObject);

	listOfObject.clear();

	//Với từng object, lấy vị trí và index, sau đó tạo coin object với vị trí và index đó,
	//tiếp theo đẩy object vào listOfObject và xóa object trong tempVector
	for (auto object : tempVector)
	{
		//Lấy vị trí và index
		object->GetIndex(indexX, indexY);
		object->GetPosition(posX, posY);

		//Tạo coin object
		tempCoin = new Coin(posX, posY, ITEM_COIN, true);
		tempCoin->SetIndex(indexX, indexY);
		tempCoin->SetDrawOrder(BLOCK_DRAW_ORDER);
		tempCoin->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
		tempCoin->SetCurrentCell(object->GetCurrentCell());
		static_cast<Coin*>(tempCoin)->SetMaster(this);
		listOfObject.push_back(tempCoin);

		//Thêm object coin vào cell và xóa object hiện tại
		tempScene->AddToCell(object->GetCurrentCell(), tempCoin);
		tempScene->Destroy(object);
	}

}

/*
	Chuyển về brick
*/
void P_Block::ChangeToBlock()
{
	LPTESTSCENE tempScene = static_cast<TestScene*>(SceneManager::GetInstance()->GetCurrentScene());
	LPGAMEOBJECT tempBlock;
	int indexX, indexY;
	float posX, posY;
	//Copy từ object holder
	vector<LPGAMEOBJECT> tempVector(listOfObject);

	listOfObject.clear();

	//Với từng object, lấy vị trí và index, sau đó tạo brick object với vị trí và index đó,
	//tiếp theo đẩy object vào listOfObject và xóa object trong tempVector
	for (auto object : tempVector)
	{
		//Lấy vị trí và index
		object->GetIndex(indexX, indexY);
		object->GetPosition(posX, posY);

		//Tạo brick object
		tempBlock = new BrickShiny(posX, posY);
		tempBlock->SetIndex(indexX, indexY);
		tempBlock->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
		tempBlock->SetAnimationSet(AnimationManager::GetInstance()->Get(ACTIVE_BLOCK));
		tempBlock->SetCurrentCell(object->GetCurrentCell());
		tempScene->Add(tempBlock);
		static_cast<BrickShiny*>(tempBlock)->SetMaster(this);
		listOfObject.push_back(tempBlock);

		//Thêm object brick vào cell và xóa object hiện tại
		tempScene->AddToCell(object->GetCurrentCell(), tempBlock);
		tempScene->Destroy(object);
	}
}

/*
	Xó object khỏi object holder
*/
void P_Block::RemoveObject(LPGAMEOBJECT objectToDelete)
{
	listOfObject.erase(std::remove(listOfObject.begin(), listOfObject.end(), objectToDelete), listOfObject.end());

}

/*
	Có 2 bounding box, 1 cái 16*16, 1 cái 16*32
*/
void P_Block::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//Có 2 ô, ô ở dưới và ô phía trên
	if (switchToP)
	{
		left = x;
		top = y - STANDARD_SIZE;
		right = left + width;
		bottom = top + height + STANDARD_SIZE;
	}
	//Chỉ 1 ô
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

	//Nếu bị hit lần đầu
	if (state == ACTIVE_BLOCK_STATE_HIT && hit == false)
	{
		oldX = currentX;
		oldY = currentY;

		switchToP = true;
		hit = true;

		moving = true;

		startMoving = GetTickCount();
	}
	//Nếu bị hit khi cục P-Block hiện ra
	else if (state == P_BLOCK_STATE_STOMP)
	{
		stomp = true;
		switchToP = false;

		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		scene->activedBlock[indexY][indexX] = true;
	}
	//Chuyển cục gạch về cục P-Block
	if (switchToP)
	{
		float distanceX = oldX - currentX;
		float distanceY = oldY - currentY;

		//Di chuyển cục gạch
		if (distanceX + distanceY != 0)
			state = P_BLOCK_STATE_MOVING;
		else
			state = P_BLOCK_STATE_NORMAL;
		

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

	//Nếu đạp P-Block thì sau 1 khoảng thời gian, coin sẽ chuyển về brick
	if (inActive)
	{
		if (GetTickCount() - activeTime > P_BLOCK_TIME_SWITCH_BACK)
		{
			ChangeToBlock();
			inActive = false;
		}
	}
	
	//DebugOut(L"size: %d \n", listOfObject.size());
}

void P_Block::Render()
{
	//Ani thuộc phần ACTIVE_BLOCK
	//3 ani tương ứng lúc chưa Active, P-Block xuất hiện và đạp P-Block

	//Đạp P-Block
	if (stomp)
	{
		sprite->Draw(x, y);
		stomp_Block->Draw(x, y - height);
	}
	//Chưa active
	else if (switchToP == false)
	{
		animation_set->Get(BRICK_SHINY_ANI)->Render(x, y);
	}
	//P-Block xuất hiện
	else if (switchToP)
	{
		p_Block->Draw(x, y - height);
		sprite->Draw(currentX, currentY);
	}
}

void P_Block::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	current->Destroy(this);
}

void P_Block::SetState(int state)
{
	GameObject::SetState(state);

	//Nếu P-Block bị đạp, chuyển object về coin
	if (state == P_BLOCK_STATE_STOMP)
	{
		ChangeToCoin();
		inActive = true;
		activeTime = GetTickCount();
	}
	//Nếu hoàn thành xong
	else if (state == P_BLOCK_STATE_DONE)
	{
		stomp = true;
		hit = true;
	}

}

P_Block::~P_Block()
{
	listOfObject.clear();

	p_Block = NULL;
	stomp_Block = NULL;
}
