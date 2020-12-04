#include "HUD.h"
#include "debug.h"

HUD::HUD(eType type)
{
	this->x = 0;
	this->y = 0;
	this->isBubble = false;
	this->number = 0;
	this->type = type;
	this->instance = GameEngine::GetInstance();
	this->global = Global::GetInstance();

	this->Setup();
	this->Add();
}

HUD::HUD(float x, float y, bool isBubble, int number, eType type)
{
	this->x = x;
	this->y = y;
	this->isBubble = isBubble;
	this->number = number;
	this->type = type;
	this->instance = GameEngine::GetInstance();
	this->global = Global::GetInstance();

	this->Setup();
	this->Add();
}

void HUD::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Add_Visual(this);
}

void HUD::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Destroy_Visual(this);
}

void HUD::Setup()
{
	if (isBubble)
	{
		vy = -HUD_BUBBLE_JUMP;
		sprite = SpriteManager::GetInstance()->Get(HUD_ID + number);
	}
	else
	{
		switch (type)
		{
		case HUD_MAIN_FRAME:
			instance->GetCamPos(x, y);

			y = y + instance->GetScreenHeight() - height;
			draw_order = HUD_FRAME_DRAW_ORDER;
			object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + HUD_FRAME));
			spriteHolder.push_back(object);

			global->frameHUD_x = x;
			global->frameHUD_y = y;
			break;
		case HUD_PLAYER_ICON:
			draw_order = HUD_TEXT_DRAW_ORDER;
			object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + global->player));
			spriteHolder.push_back(object);

			break;
		case HUD_PLAYER_LIVE:
		{
			draw_order = HUD_TEXT_DRAW_ORDER;

			oldNumber = global->live;
			auto listNumber = numberToList(global->live, 2);

			for (auto number : listNumber)
			{
				object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number)));
				spriteHolder.push_back(object);
			}

		}

			break;
		case HUD_WORLD:
			draw_order = HUD_TEXT_DRAW_ORDER;
			object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + ('0' + global->world)));
			spriteHolder.push_back(object);

			break;
		case HUD_SPEEDOMETER:
			draw_order = HUD_TEXT_DRAW_ORDER;

			for (int i = 0; i < 6; ++i)
			{
				object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + HUD_ICON_ARROW_DARK));
				spriteHolder.push_back(object);
			}

			object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + HUD_ICON_BUTTON_DARK));
			spriteHolder.push_back(object);

			break;
		case HUD_POINT:
		{
			draw_order = HUD_TEXT_DRAW_ORDER;

			oldNumber = global->point;
			auto listNumber = numberToList(global->point, 7);

			for (auto number : listNumber)
			{
				object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number)));
				spriteHolder.push_back(object);
			}
		}
			
			break;
		case HUD_MONEY:
		{
			draw_order = HUD_TEXT_DRAW_ORDER;

			oldNumber = global->money;
			auto listNumber = numberToList(global->money, 2);

			for (auto number : listNumber)
			{
				object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number)));
				spriteHolder.push_back(object);
			}

		}
			break;
		case HUD_TIME:
		{
			draw_order = HUD_TEXT_DRAW_ORDER;

			oldNumber = global->time;
			auto listNumber = numberToList(global->time, 3);

			for (auto number : listNumber)
			{
				object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number)));
				spriteHolder.push_back(object);
			}

		}
			break;
		case HUD_CARD_ONE:
			draw_order = HUD_TEXT_DRAW_ORDER;
			object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + global->cardOne));
			spriteHolder.push_back(object);

			break;
		case HUD_CARD_TWO:
			draw_order = HUD_TEXT_DRAW_ORDER;
			object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + global->cardTwo));
			spriteHolder.push_back(object);

			break;
		case HUD_CARD_THREE:
			draw_order = HUD_TEXT_DRAW_ORDER;
			object = new HUD_Object(SpriteManager::GetInstance()->Get(HUD_ID + global->cardThree));
			spriteHolder.push_back(object);

			break;
		default:
			break;
		}

		this->width = object->GetSprite()->GetWidth();
		this->height = object->GetSprite()->GetHeight();
	}
	/*
	if (!sprite)
		DebugOut(L"[ERROR] HUD sprites empty!!! \n");*/

}

vector<int> HUD::numberToList(int number, int n)
{
	vector<int> listNumber;

	while (number != 0)
	{
		listNumber.push_back(number % 10);
		number /= 10;
	}

	if (listNumber.size() < n)
	{
		int remain = n - listNumber.size();

		while (remain != 0)
		{
			listNumber.push_back(0);
			--remain;
		}
	}

	reverse(listNumber.begin(), listNumber.end());

	return listNumber;
}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isBubble)
	{
		GameObject::Update(dt);

		vy += HUD_BUBBLE_GRAVITY * dt;

		if (vy >= 0)
		{
			this->Destroy();
			return;
		}
					
	}
	else
	{
		switch (type)
		{
		case HUD_MAIN_FRAME:
			instance->GetCamPos(x, y);

			y = y + instance->GetScreenHeight() - height;
			spriteHolder.front()->SetPosition(x, y);

			global->frameHUD_x = x;
			global->frameHUD_y = y;
			break;
		case HUD_PLAYER_ICON:
			x = global->frameHUD_x + HUD_PLAYER_ICON_X;
			y = global->frameHUD_y + HUD_PLAYER_ICON_Y;
			spriteHolder.front()->SetPosition(x, y);

			break;
		case HUD_PLAYER_LIVE:
		{
			x = global->frameHUD_x + HUD_PLAYER_LIVE_X;
			y = global->frameHUD_y + HUD_PLAYER_LIVE_Y;

			if (oldNumber != global->live)
			{
				oldNumber = global->live;

				auto listNumber = numberToList(global->live, 2);
				int i = 0;

				for (auto number : listNumber)
				{
					spriteHolder[i]->SetSprite(SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number)));
					++i;
				}

			}
			int i = 0;

			for (auto object : spriteHolder)
			{
				object->SetPosition(x + i * object->GetSprite()->GetWidth(), y);
				++i;
			}
		}
			
			break;
		case HUD_WORLD:
			x = global->frameHUD_x + HUD_WORLD_X;
			y = global->frameHUD_y + HUD_WORLD_Y;

			spriteHolder.front()->SetPosition(x, y);

			break;
		case HUD_SPEEDOMETER:
		{
			x = global->frameHUD_x + HUD_SPEED_O_METER_X;
			y = global->frameHUD_y + HUD_SPEED_O_METER_Y;

			int limit = global->speed;

			for (int i = 0; i < 6; ++i)
			{
				if (i < limit)
					spriteHolder[i]->SetSprite(SpriteManager::GetInstance()->Get(HUD_ID + HUD_ICON_ARROW_BRIGHT));
				else
					spriteHolder[i]->SetSprite(SpriteManager::GetInstance()->Get(HUD_ID + HUD_ICON_ARROW_DARK));
				spriteHolder[i]->SetPosition(x + i * HUD_ARROW_SPACING, y);
			}

			if (limit == 7)
			{
				if (GetTickCount() - buttonFlashing < HUD_BUTTON_FLASH_TIME)
				{
					spriteHolder[6]->SetSprite(SpriteManager::GetInstance()->Get(HUD_ID + HUD_ICON_BUTTON_BRIGHT));
				}
				else
				{
					spriteHolder[6]->SetSprite(SpriteManager::GetInstance()->Get(HUD_ID + HUD_ICON_BUTTON_DARK));

					if (GetTickCount() - buttonFlashing > HUD_BUTTON_FLASH_TIME_2)
						buttonFlashing = GetTickCount();
				}
					
			}
			else
				spriteHolder[6]->SetSprite(SpriteManager::GetInstance()->Get(HUD_ID + HUD_ICON_BUTTON_DARK));
				
			spriteHolder[6]->SetPosition(x + 6 * HUD_ARROW_SPACING, y);
		}	

			break;
		case HUD_POINT:
		{
			x = global->frameHUD_x + HUD_POINT_X;
			y = global->frameHUD_y + HUD_POINT_Y;

			if (oldNumber != global->point)
			{
				oldNumber = global->point;

				auto listNumber = numberToList(global->point, 7);
				int i = 0;

				for (auto number : listNumber)
				{
					spriteHolder[i]->SetSprite(SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number)));
					++i;
				}

			}
			int i = 0;

			for (auto object : spriteHolder)
			{
				object->SetPosition(x + i * object->GetSprite()->GetWidth(), y);
				++i;
			}
		}
			
			break;
		case HUD_MONEY:
		{
			x = global->frameHUD_x + HUD_MONEY_X;
			y = global->frameHUD_y + HUD_MONEY_Y;

			if (oldNumber != global->money)
			{
				oldNumber = global->money;

				auto listNumber = numberToList(global->money, 2);
				int i = 0;

				for (auto number : listNumber)
				{
					spriteHolder[i]->SetSprite(SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number)));
					++i;
				}

			}
			int i = 0;

			for (auto object : spriteHolder)
			{
				object->SetPosition(x + i * object->GetSprite()->GetWidth(), y);
				++i;
			}
		}
			break;
		case HUD_TIME:
		{
			x = global->frameHUD_x + HUD_TIME_X;
			y = global->frameHUD_y + HUD_TIME_Y;

			if (oldNumber != global->time)
			{
				oldNumber = global->time;

				auto listNumber = numberToList(global->time, 3);
				int i = 0;

				for (auto number : listNumber)
				{
					spriteHolder[i]->SetSprite(SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number)));
					++i;
				}

			}
			int i = 0;

			for (auto object : spriteHolder)
			{
				object->SetPosition(x + i * object->GetSprite()->GetWidth(), y);
				++i;
			}
		}
			break;
		case HUD_CARD_ONE:
			x = global->frameHUD_x + HUD_CARD_ONE_X;
			y = global->frameHUD_y + HUD_CARD_ONE_Y;
			spriteHolder.front()->SetPosition(x, y);

			break;
		case HUD_CARD_TWO:
			x = global->frameHUD_x + HUD_CARD_TWO_X;
			y = global->frameHUD_y + HUD_CARD_TWO_Y;
			spriteHolder.front()->SetPosition(x, y);

			break;
		case HUD_CARD_THREE:
			x = global->frameHUD_x + HUD_CARD_THREE_X;
			y = global->frameHUD_y + HUD_CARD_THREE_Y;
			spriteHolder.front()->SetPosition(x, y);

			break;
		default:
			break;
		}
	}
}

void HUD::Render()
{
	if (isBubble)
	{
		sprite->Draw(x, y);
	}
	else
	{
		switch (type)
		{
		case HUD_MAIN_FRAME:
			spriteHolder.front()->Render();
			break;
		case HUD_PLAYER_ICON:
			spriteHolder.front()->Render();
			break;
		case HUD_PLAYER_LIVE:
			for (auto sprite : spriteHolder)
				sprite->Render();
			break;
		case HUD_WORLD:
			spriteHolder.front()->Render();
			break;
		case HUD_SPEEDOMETER:
			for (auto sprite : spriteHolder)
				sprite->Render();
			break;
		case HUD_POINT:
			for (auto sprite : spriteHolder)
				sprite->Render();
			break;
		case HUD_MONEY:
			for (auto sprite : spriteHolder)
				sprite->Render();
			break;
		case HUD_TIME:
			for (auto sprite : spriteHolder)
				sprite->Render();
			break;
		case HUD_CARD_ONE:
			spriteHolder.front()->Render();
			break;
		case HUD_CARD_TWO:
			spriteHolder.front()->Render();
			break;
		case HUD_CARD_THREE:
			spriteHolder.front()->Render();
			break;
		default:
			break;
		}
	}
}

HUD::~HUD()
{
	spriteHolder.clear();
}
