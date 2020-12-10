#include "Intro.h"
#include "debug.h"

Intro::Intro(int id, LPCWSTR filePath) : TestScene(id, filePath)
{
}

void Intro::Load()
{
	TestScene::Load();
	Global::GetInstance()->background_color = D3DCOLOR_XRGB(0, 0, 0);
	AUTO = true;

	int i, j;
	float x, y;

	for (auto object : gameObjects)
	{
		object->GetIndex(i, j);

		if (i == 2 && j == 0)
		{
			ribbon_top = static_cast<BackGround*>(object);
			ribbon_top->SetType(eType::MENU_TITLE);
			ribbon_top->SetDrawOrder(MENU_DRAW_ORDER_RIBBON_BG);
		}
		else if (i == 2 && j == 1)
		{
			ribbon = static_cast<BackGround*>(object);
			ribbon->SetType(eType::MENU_TITLE);
			ribbon->GetPosition(x, y);
			ribbon->SetPosition(x, y - STANDARD_SIZE);
			ribbon->SetDrawOrder(MENU_DRAW_ORDER_RIBBON);
		}
		else if (i == 2 && j == 3)
		{
			title = static_cast<BackGround*>(object);
			title->SetType(eType::MENU_TITLE);
			title->GetPosition(x, y);
			title->SetPosition(x, y - STANDARD_SIZE * 12);
			title->SetDrawOrder(MENU_DRAW_ORDER_TITLE);
		}
		else if (i == 2 && j == 7)
		{
			tree1 = static_cast<BackGround*>(object);
			tree1->SetType(eType::MENU_TITLE);
			tree1->GetPosition(x, y);
			tree1->SetPosition(x, y + STANDARD_SIZE / 2);
			tree1->SetDrawOrder(MENU_DRAW_ORDER_TREE);
		}
		else if (i == 14 && j == 5)
		{
			tree2 = static_cast<BackGround*>(object);
			tree2->SetType(eType::MENU_TITLE);
			tree2->GetPosition(x, y);
			tree2->SetPosition(x, y + STANDARD_SIZE / 2);
			tree2->SetDrawOrder(MENU_DRAW_ORDER_TREE);
		}
		else if (i == 2 && j == 12)
		{
			object->SetType(eType::MENU_TITLE);
			object->GetPosition(x, y);
			object->SetPosition(x, y + STANDARD_SIZE / 2);
			object->SetDrawOrder(MENU_DRAW_ORDER_RIBBON_BG);
		}
		else if (i == 9 && j == 6)
		{
			number = static_cast<BackGround*>(object);
			number->SetType(eType::MENU_TITLE);
			number->SetAnimationSet(AnimationManager::GetInstance()->Get(MENU));
			number->GetPosition(x, y);
			number->SetPosition(x, y - STANDARD_SIZE * 10);
			number->SetDrawOrder(MENU_DRAW_ORDER_TITLE);
		}
		else
		{
			if (dynamic_cast<GroupObject*>(object))
			{
				GroupObject* group = static_cast<GroupObject*>(object);
				group->Move(0, STANDARD_SIZE / 2);
				group->SetDrawOrder(MENU_DRAW_ORDER_RIBBON_BG);
			}
		}
	}


	GameEngine* game = GameEngine::GetInstance();
	GameEngine::GetInstance()->SetCamPos(32.0f, 0.0f);

	ribbon_top->SetDisableDraw();
	tree1->SetDisableDraw();
	tree2->SetDisableDraw();
	number->SetDisableDraw();
}

void Intro::Update(DWORD dt)
{
	TestScene::Update(dt);
	DWORD now = GetTickCount();
	
	if (moveRibbon)
	{
		float x, y;
		ribbon->GetPosition(x, y);
	
		if (setSpeed)
		{
			ribbon->SetSpeed(0, -MENU_RIBBON_MOVE_Y);
			setSpeed = false;
		}

		
		if (y < -MENU_RIBBON_LIMIT)
		{
			moveRibbon = false;
			ribbon->SetSpeed(0, 0);

			moveTitle = true;
			setSpeed = true;
		}
			
	}
	else if (moveTitle)
	{
		float x, y;
		float xNum, yNum;
		title->GetPosition(x, y);
		number->GetPosition(xNum, yNum);

		if (setSpeed)
		{
			title->SetSpeed(0, MENU_TITLE_MOVE_Y);
			number->SetSpeed(0, MENU_TITLE_MOVE_Y);
			setSpeed = false;
		}

		if (y > MENU_TITLE_Y && shaking == false)
		{
			title->SetSpeed(0, 0);
			number->SetSpeed(0, 0);
			shaking = true;
		}
		
		else if (shaking)
		{
			if (startShaking)
			{
				shakingTime = now;
				startShaking = false;
			}

			if (now - shakingTime > MENU_TITLE_MAX_TIME)
			{
				moveTitle = false;
				shaking = false;

				title->SetSpeed(0, 0);
				number->SetSpeed(0, 0);

				title->SetPosition(x, MENU_TITLE_Y);
				number->SetPosition(xNum, MENU_NUMBER_Y);

				allowTranform = true;
			}
		}
		
	}
	else if (allowTranform)
	{
		if (startTranform)
		{
			tranformTime = now;
			startTranform = false;
		}

		DWORD timePass = now - tranformTime;
		D3DCOLOR color;

		if (timePass > MENU_TIME_STAGE_FOUR)
		{
			color = D3DCOLOR_XRGB(255, 255, 255);
			Global::GetInstance()->background_color = D3DCOLOR_XRGB(252, 216, 168);
			allowTranform = false;

			title->SetDrawColor(color);
			tree1->SetDrawColor(color);
			tree2->SetDrawColor(color);
			number->SetDrawColor(color);
			ribbon_top->SetDrawColor(color);

			number->SetAllowDraw();

		}
		else if (timePass > MENU_TIME_STAGE_THREE)
		{
			color = D3DCOLOR_XRGB(200, 200, 200);
			Global::GetInstance()->background_color = D3DCOLOR_XRGB(252, 152, 56);

			title->SetDrawColor(color);
			tree1->SetDrawColor(color);
			tree2->SetDrawColor(color);
			number->SetDrawColor(color);
			ribbon_top->SetDrawColor(color);
		}
		else if (timePass > MENU_TIME_STAGE_TWO)
		{
			color = D3DCOLOR_XRGB(100, 100, 100);
			Global::GetInstance()->background_color = D3DCOLOR_XRGB(200, 76, 12);

			title->SetDrawColor(color);
			tree1->SetDrawColor(color);
			tree2->SetDrawColor(color);
			number->SetDrawColor(color);
			ribbon_top->SetDrawColor(color);
		}
		else if (timePass > MENU_TIME_STAGE_ONE)
		{
			color = D3DCOLOR_XRGB(50, 50, 50);
			Global::GetInstance()->background_color = D3DCOLOR_XRGB(124, 8, 0);

			ribbon_top->SetAllowDraw();
			tree1->SetAllowDraw();
			tree2->SetAllowDraw();

			title->SetSprite(SpriteManager::GetInstance()->Get(MENU_TITLE_TEXT_BRIGHT));

			title->SetDrawColor(color);
			tree1->SetDrawColor(color);
			tree2->SetDrawColor(color);
			number->SetDrawColor(color);
			ribbon_top->SetDrawColor(color);
		}

	}


}

void Intro::Render()
{
	if (shaking)
	{
		sign = -sign;

		title->GetPosition(shakeX, shakeY);
		number->GetPosition(shakeXNum, shakeYNum);

		title->SetPosition(shakeX, shakeY + sign * MENU_TITLE_VIBRATION);
		number->SetPosition(shakeXNum, shakeYNum + sign * MENU_TITLE_VIBRATION);

	}

	TestScene::Render();
}

void Intro::Unload()
{
}
