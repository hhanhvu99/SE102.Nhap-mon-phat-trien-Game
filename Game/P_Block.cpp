#include "P_Block.h"

P_Block::P_Block(float x, float y) : GameObject()
{
	this->x = x;
	this->y = y;
	this->width = STANDARD_SIZE;
	this->height = STANDARD_SIZE;

	this->state = P_BLOCK_STATE_NORMAL;
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

void P_Block::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == P_BLOCK_STATE_HIT)
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

}

void P_Block::Render()
{
	if (state == P_BLOCK_STATE_NORMAL)
	{
		animation_set->Get(BRICK_SHINY_ANI)->Render(x, y);
	}
	else if (state == P_BLOCK_STATE_HIT)
	{
		sprite->Draw(x, y);
		p_Block->Draw(x, y - height);
	}
	else
	{
		sprite->Draw(x, y);
		stomp_Block->Draw(x, y - height);
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
