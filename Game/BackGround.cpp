#include "BackGround.h"

BackGround::BackGround(float x, float y, LPSPRITE sprite)
{
	this->x = x;
	this->y = y;
	this->sprite = sprite;
	this->width = BACKGROUND_BLOCK_WIDTH;
	this->height = BACKGROUND_BLOCK_HEIGHT;
	this->type = eType::BACKGROUND;
	this->allowDraw = true;
	this->color = D3DCOLOR_XRGB(255, 255, 255);
}

void BackGround::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	GameObject::Update(dt);
	x += dx;
	y += dy;
}


void BackGround::Render()
{
	if (type == eType::BACKGROUND)
		this->sprite->Draw(x, y);
	else if (type == eType::MAP_TREE)
		animation_set->Get(MAP_TREE_ANIMATION_ID)->Render(x, y);
	else if (type == eType::MAP_POPUP)
	{
		if (allowDraw)
			this->sprite->Draw(x, y);
	}
	else
	{
		if (indexX == 9 && indexY == 6)
		{
			if (allowDraw)
			{
				animation_set->Get(MENU_ANI_NUMBER)->Render(x, y);
			}
			else
			{
				this->sprite->Draw(x, y, color);
			}
		}
		else
		{
			if (allowDraw)
				this->sprite->Draw(x, y, color);
		}
			
	}
}

void BackGround::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	current->Destroy_Visual(this);
}

