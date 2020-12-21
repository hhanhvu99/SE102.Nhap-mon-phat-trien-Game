#include "WorldMap.h"

#include "WorldMap.h"
#include "debug.h"

WorldMap::WorldMap(int id, LPCWSTR filePath) : TestScene(id, filePath)
{
	this->type = 3;
	this->current = NULL;
	global->level = MARIO_LEVEL_SMALL;
}

void WorldMap::Reset()
{
	castMario->MoveTo(startX, startY);
	current = listOfPath[Global::TwoDimension_To_OneDimension(int(startX / 16), int(startY / 16), height)];

}

void WorldMap::Restart()
{
	castMario->MoveTo(startX, startY);
	castMario->SetState(MARIO_MAP_STATE_ROLLING);
	current = listOfPath[Global::TwoDimension_To_OneDimension(int(startX / 16), int(startY / 16), height)];
}

void WorldMap::Load()
{
	CHOOSE = 3;
	TestScene::Load();
	Global::GetInstance()->background_color = D3DCOLOR_XRGB(248, 236, 160);

	global->allowCountTime = false;
	showPopup = false;
	startTime = true;
	timePass = 0;

	gameOver = false;
	firstOver = true;
	firstOption = true;

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
			if (current == NULL)
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
	
	//Find current path
	if (allowResetStart == false)
		current = listOfPath[Global::TwoDimension_To_OneDimension(global->currentX, global->currentY, height)];

	//Setup Mario
	mario = new MarioMap(startPosX, startPosY);
	castMario = static_cast<MarioMap*>(mario);
	castMario->SetAnimationSet(AnimationManager::GetInstance()->Get(MARIO));
	castMario->SetDrawOrder(MAP_DRAW_ORDER_PLAYER);
	gameObjects.push_back(castMario);
	collideObjects.push_back(castMario);

	GameEngine::GetInstance()->SetCamPos(0.0f, 0.0f);

	PAUSE = true;
	allowResetStart = false;
	Global::GetInstance()->inWorldMap = true;
	Global::GetInstance()->time = 0;
}

void WorldMap::Update(DWORD dt)
{
	TestScene::Update(dt);
	DWORD now = GetTickCount();

	if(!castMario->IsMoving())
		castMario->GetPosition(startPosX, startPosY);


	if (global->die)
	{
		global->live -= 1;

		if (global->live > 0)
			Reset();
		else
			gameOver = true;
			
		global->die = false;
	}
	else if (global->finished)
	{
		int indexX, indexY;

		current->currentPath->SetSprite(SpriteManager::GetInstance()->Get(MAP_MARIO_FINISHED));
		current->currentPath->GetIndex(indexX, indexY);
		current->isFinished = true;
		STAGE_FINISHED.push_back(indexX);
		STAGE_FINISHED.push_back(indexY);

		global->finished = false;
	}
	
	
	if (gameOver)
	{
		if (firstOver)
		{
			timePass = now;
			firstOver = false;

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
			PAUSE = true;
			if (firstOption)
				arrow->SetPosition(MAP_POS_ARROW_1_X, MAP_POS_ARROW_1_Y);
			else
				arrow->SetPosition(MAP_POS_ARROW_2_X, MAP_POS_ARROW_2_Y);
		}
	}
	else if (startTime)
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

		auto number = HUD::numberToString(global->live, 2);
		numberOne = new BackGround(160.0f, 72.0f, SpriteManager::GetInstance()->Get(HUD_ID + int(number[0])));
		numberOne->SetType(eType::MAP_START);
		numberOne->SetDrawOrder(HUD_TEXT_DRAW_ORDER);
		this->gameObjects.push_back(numberOne);
		numberTwo = new BackGround(168.0f, 72.0f, SpriteManager::GetInstance()->Get(HUD_ID + int(number[1])));
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
		else
			PAUSE = true;
	}
	
}

void WorldMap::Render()
{

	//DebugOut(L"Render Size: %d\n", gameObjects.size());

	TestScene::Render();
}

void WorldMap::Unload()
{
	TestScene::Unload();

}

void WorldMap::SetState(int state)
{
	Scene::SetState(state);

	switch (state)
	{
	case SCENE_STATE_SWITCH:
		if (global->allowSwitch)
		{
			PAUSE = true;
			Teleport* gate = static_cast<Teleport*>(currentGate);
			SceneManager::GetInstance()->SwitchScene(gate->GetTargetID());
		}
		break;
	case MAP_STATE_MOVE_UP:
		if (!castMario->IsMoving())
		{
			if (current->adjacent[0] != 0 && current->adjacent[1] != 0)
			{
				castMario->MoveTo(current->adjacent[0] * STANDARD_SIZE, current->adjacent[1] * STANDARD_SIZE);
				current = listOfPath[Global::TwoDimension_To_OneDimension(current->adjacent[0], current->adjacent[1], height)];
				current->currentPath->GetIndex(global->currentX, global->currentY);
			}
		}
		break;
	case MAP_STATE_MOVE_DOWN:
		if (!castMario->IsMoving())
		{
			if (current->adjacent[2] != 0 && current->adjacent[3] != 0)
			{
				castMario->MoveTo(current->adjacent[2] * STANDARD_SIZE, current->adjacent[3] * STANDARD_SIZE);
				current = listOfPath[Global::TwoDimension_To_OneDimension(current->adjacent[2], current->adjacent[3], height)];
				current->currentPath->GetIndex(global->currentX, global->currentY);
			}
		}
		break;
	case MAP_STATE_MOVE_LEFT:
		if (!castMario->IsMoving())
		{
			if (current->adjacent[4] != 0 && current->adjacent[5] != 0)
			{
				castMario->MoveTo(current->adjacent[4] * STANDARD_SIZE, current->adjacent[5] * STANDARD_SIZE);
				current = listOfPath[Global::TwoDimension_To_OneDimension(current->adjacent[4], current->adjacent[5], height)];
				current->currentPath->GetIndex(global->currentX, global->currentY);
			}
		}
		break;
	case MAP_STATE_MOVE_RIGHT:
		if (!castMario->IsMoving())
		{
			if (current->adjacent[6] != 0 && current->adjacent[7] != 0)
			{
				castMario->MoveTo(current->adjacent[6] * STANDARD_SIZE, current->adjacent[7] * STANDARD_SIZE);
				current = listOfPath[Global::TwoDimension_To_OneDimension(current->adjacent[6], current->adjacent[7], height)];
				current->currentPath->GetIndex(global->currentX, global->currentY);
			}
		}
		break;
	case MAP_STATE_RESET:
		if (!castMario->IsMoving())
			Reset();
		break;
	case MAP_STATE_RESTART:
		if (!castMario->IsMoving())
			gameOver = true;	
		break;
	case MAP_STATE_SELECT:
		if (gameOver)
		{
			firstOption = !firstOption;
		}
		break;
	case MAP_STATE_CHOOSE:
		if (gameOver && firstOption)
		{
			gameOver = false;
			firstOver = true;
			PAUSE = false;

			Global::GetInstance()->Setup();

			Restart();

			Destroy(text);
			Destroy(arrow);
		}
		break;
	case SCENE_STATE_MAP_TO_STAGE:
	{
		if (current->isFinished)
			return;

		Teleport* gate = static_cast<Teleport*>(currentGate);
		SceneManager::GetInstance()->SwitchScene(gate->GetTargetID());
		//SceneManager::GetInstance()->SwitchScene(SCENE_WORLD_1_1);
	}
		
		break;
	default:
		break;
	}

}
