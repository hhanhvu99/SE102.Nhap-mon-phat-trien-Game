#include "WorldMap.h"

#include "WorldMap.h"
#include "debug.h"

WorldMap::WorldMap(int id, LPCWSTR filePath) : TestScene(id, filePath)
{
	this->type = 3;
	global = Global::GetInstance();
	global->level = MARIO_LEVEL_SMALL;
}

void WorldMap::Reset()
{
	mario->MoveTo(startX, startY);
	current = listOfPath[Global::TwoDimension_To_OneDimension(int(startX / 16), int(startY / 16), height)];

}

void WorldMap::Restart()
{
	mario->MoveTo(startX, startY);
	mario->SetState(MARIO_MAP_STATE_ROLLING);
	current = listOfPath[Global::TwoDimension_To_OneDimension(int(startX / 16), int(startY / 16), height)];
}

void WorldMap::Load()
{
	CHOOSE = 3;
	TestScene::Load();
	Global::GetInstance()->background_color = D3DCOLOR_XRGB(248, 236, 160);

	//Setup Path
	int pathType;
	int indexX, indexY;
	int indexObj_x, indexObj_y;
	LPPATH path = NULL;

	int length = PATH.size();
	for (int x = 0; x < length; x += 11)
	{
		indexX = PATH[x];
		indexY = PATH[x + 1];
		pathType = PATH[x + 10];
		path = new Path();

		switch (pathType)
		{
		case MAP_PATH_START:
			startX = indexX * STANDARD_SIZE;
			startY = indexY * STANDARD_SIZE;
			global->currentX = indexX;
			global->currentY = indexY;
			current = path;
			break;
		case MAP_PATH_STAGE_ONE:
		case MAP_PATH_STAGE_TWO:	
		case MAP_PATH_STAGE_THREE:
		case MAP_PATH_STAGE_FOUR:
		case MAP_PATH_STAGE_FIVE:
		case MAP_PATH_STAGE_SIX:
		case MAP_PATH_GAMBLE_ONE:
		case MAP_PATH_GAMBLE_TWO:
		case MAP_PATH_CASTLE_MID:
		case MAP_PATH_CASTLE_TOAST:
		case MAP_PATH_ROAD:
		case MAP_PATH_CASTLE:
			break;
		default:
			DebugOut(L"[ERROR] Unknown path type: %d\n", pathType);
		}

		path->type = pathType;

		for (int i = 0; i < 8; ++i)
			path->adjacent[i] = PATH[x + 2 + i];

		for (auto object : gameObjects)
		{
			object->GetIndex(indexObj_x, indexObj_y);
			if (indexObj_x == indexX && indexObj_y == indexY)
			{
				BackGround* newObject = static_cast<BackGround*>(object);
				path->currentPath = newObject;
			}
		}

		listOfPath[Global::TwoDimension_To_OneDimension(indexX, indexY, height)] = path;
	}


	//Setup Mario
	mario = new MarioMap(startX, startY);
	mario->SetAnimationSet(AnimationManager::GetInstance()->Get(MARIO));
	mario->SetDrawOrder(MAP_DRAW_ORDER_PLAYER);
	gameObjects.push_back(mario);
	collideObjects.push_back(mario);

	GameEngine::GetInstance()->SetCamPos(0.0f, 0.0f);

	PAUSE = true;
}

void WorldMap::Update(DWORD dt)
{
	TestScene::Update(dt);
	DWORD now = GetTickCount();
	
	if (startTime)
	{
		showPopup = true;
		timePass = now;
		startTime = false;

		startScene = new BackGround(64.0f, 32.0f, SpriteManager::GetInstance()->Get(HUD_ID + HUD_START));
		startScene->SetType(eType::MAP_START);
		startScene->SetDrawOrder(HUD_FRAME_DRAW_ORDER);
		this->gameObjects.push_back(startScene);

		world = new BackGround(145.0f, 48.0f, SpriteManager::GetInstance()->Get(HUD_ID + ('0' + global->world)));
		world->SetType(eType::MAP_START);
		world->SetDrawOrder(HUD_TEXT_DRAW_ORDER);
		this->gameObjects.push_back(world);

		auto number = HUD::numberToList(global->live, 2);
		numberOne = new BackGround(160.0f, 72.0f, SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number[0])));
		numberOne->SetType(eType::MAP_START);
		numberOne->SetDrawOrder(HUD_TEXT_DRAW_ORDER);
		this->gameObjects.push_back(numberOne);
		numberTwo = new BackGround(168.0f, 72.0f, SpriteManager::GetInstance()->Get(HUD_ID + ('0' + number[1])));
		numberTwo->SetType(eType::MAP_START);
		numberTwo->SetDrawOrder(HUD_TEXT_DRAW_ORDER);
		this->gameObjects.push_back(numberTwo);
	}
	else if (showPopup)
	{
		if (now - timePass > MAP_POPUP)
		{
			Destroy(startScene);
			Destroy(world);
			Destroy(numberOne);
			Destroy(numberTwo);
			PAUSE = false;
			showPopup = false;
		}
	}
	else if (gameOver)
	{
		if (firstOver)
		{
			timePass = now;
			firstOver = false;
			PAUSE = true;

			text = new BackGround(64.0f, 32.0f, SpriteManager::GetInstance()->Get(HUD_ID + HUD_GAMEOVER));
			text->SetType(eType::MAP_START);
			text->SetDrawOrder(HUD_FRAME_DRAW_ORDER);
			this->gameObjects.push_back(text);

			arrow = new BackGround(MAP_POS_ARROW_1_X, MAP_POS_ARROW_1_Y, SpriteManager::GetInstance()->Get(MENU_ARROW));
			arrow->SetType(eType::MAP_START);
			arrow->SetDrawOrder(HUD_TEXT_DRAW_ORDER);
			this->gameObjects.push_back(arrow);
		}
		else
		{
			if (firstOption)
				arrow->SetPosition(MAP_POS_ARROW_1_X, MAP_POS_ARROW_1_Y);
			else
				arrow->SetPosition(MAP_POS_ARROW_2_X, MAP_POS_ARROW_2_Y);
		}
	}
}

void WorldMap::Render()
{

	//DebugOut(L"Render Size: %d\n", gameObjects.size());

	TestScene::Render();
}

void WorldMap::Unload()
{
	for (auto object : gameObjects)
		Destroy(object);

	

}

void WorldMap::SetState(int state)
{
	Scene::SetState(state);

	switch (state)
	{
	case MAP_STATE_MOVE_UP:
		if (!mario->IsMoving())
		{
			if (current->adjacent[0] != 0 && current->adjacent[1] != 0)
			{
				mario->MoveTo(current->adjacent[0] * STANDARD_SIZE, current->adjacent[1] * STANDARD_SIZE);
				current = listOfPath[Global::TwoDimension_To_OneDimension(current->adjacent[0], current->adjacent[1], height)];
			}
		}
		break;
	case MAP_STATE_MOVE_DOWN:
		if (!mario->IsMoving())
		{
			if (current->adjacent[2] != 0 && current->adjacent[3] != 0)
			{
				mario->MoveTo(current->adjacent[2] * STANDARD_SIZE, current->adjacent[3] * STANDARD_SIZE);
				current = listOfPath[Global::TwoDimension_To_OneDimension(current->adjacent[2], current->adjacent[3], height)];
			}
		}
		break;
	case MAP_STATE_MOVE_LEFT:
		if (!mario->IsMoving())
		{
			if (current->adjacent[4] != 0 && current->adjacent[5] != 0)
			{
				mario->MoveTo(current->adjacent[4] * STANDARD_SIZE, current->adjacent[5] * STANDARD_SIZE);
				current = listOfPath[Global::TwoDimension_To_OneDimension(current->adjacent[4], current->adjacent[5], height)];
			}
		}
		break;
	case MAP_STATE_MOVE_RIGHT:
		if (!mario->IsMoving())
		{
			if (current->adjacent[6] != 0 && current->adjacent[7] != 0)
			{
				mario->MoveTo(current->adjacent[6] * STANDARD_SIZE, current->adjacent[7] * STANDARD_SIZE);
				current = listOfPath[Global::TwoDimension_To_OneDimension(current->adjacent[6], current->adjacent[7], height)];
			}
		}
		break;
	case MAP_STATE_RESET:
		if (!mario->IsMoving())
			Reset();
		break;
	case MAP_STATE_RESTART:
		if (!mario->IsMoving())
			gameOver = true;	
		break;
	case MAP_STATE_SELECT:
		if (gameOver)
		{
			firstOption = !firstOption;
		}
		break;
	case MAP_STATE_CHOOSE:
		if (gameOver)
		{
			gameOver = false;
			firstOver = true;
			PAUSE = false;
			Restart();

			Destroy(text);
			Destroy(arrow);
		}
		break;
	default:
		break;
	}

}
