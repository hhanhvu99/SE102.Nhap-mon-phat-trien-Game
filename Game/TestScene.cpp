#include <iostream>
#include <fstream>
#include <algorithm>

#include "TestScene.h"
#include "debug.h"

TestScene::TestScene(int id, LPCWSTR filePath) : Scene(id, filePath)
{
	global = Global::GetInstance();
}

void TestScene::CheckCell()
{
	float camX, camY, endCamX, endCamY;
	float startCellX, startCellY, endCellX, endCellY;
	GameEngine::GetInstance()->GetCamPos(camX, camY);

	endCamX = camX + SCREEN_WIDTH + ENTITY_SAFE_DELETE_RANGE;
	endCamY = camY + SCREEN_HEIGHT + ENTITY_SAFE_DELETE_RANGE;
	camX -= ENTITY_SAFE_DELETE_RANGE;
	camY -= ENTITY_SAFE_DELETE_RANGE;

	for (auto cell : cells)
	{
		LPGRID cell_cast = static_cast<LPGRID>(cell);
		cell_cast->GetCellPos(startCellX, startCellY, endCellX, endCellY);
		if (camX < endCellX && endCamX > startCellX && camY < endCellY && endCamY > startCellY)
		{
			if (global->cells.find(cell_cast->GetID()) == global->cells.end())
				cell_cast->Load(gameObjects, collideObjects);
		}
		else
		{
			if (global->cells.find(cell_cast->GetID()) != global->cells.end())
				cell_cast->Unload();
				
		}
		
	}

}

void TestScene::Add(LPGAMEOBJECT gameObject)
{
	addList.push_back(gameObject);
	collideObjects.push_back(gameObject);

}

void TestScene::Destroy(LPGAMEOBJECT gameObject)
{
	auto objectDelete = std::find(collideObjects.begin(), collideObjects.end(), gameObject);
	if (objectDelete != collideObjects.end())
		collideObjects.erase(objectDelete);

	deleteList.push_back(gameObject);

	if (gameObject->GetCurrentCell() != -1)
		RemoveFromCell(gameObject->GetCurrentCell(), gameObject);
}

void TestScene::Remove(LPGAMEOBJECT gameObject)
{
	auto objectDelete = std::find(collideObjects.begin(), collideObjects.end(), gameObject);
	if (objectDelete != collideObjects.end())
		collideObjects.erase(objectDelete);
}

void TestScene::Add_Visual(LPGAMEOBJECT gameObject)
{
	addList.push_back(gameObject);
}

void TestScene::Destroy_Visual(LPGAMEOBJECT gameObject)
{
	deleteList.push_back(gameObject);

	if (gameObject->GetCurrentCell() != -1)
		RemoveFromCell(gameObject->GetCurrentCell(), gameObject);
}

void TestScene::AddToCell(int cell, LPGAMEOBJECT gameObject)
{
	static_cast<LPGRID>(cells[cell])->Insert(gameObject);
}

void TestScene::RemoveFromCell(int cell, LPGAMEOBJECT gameObject)
{
	static_cast<LPGRID>(cells[cell])->Delete(gameObject);
}

void TestScene::GetMarioPos(float& x, float& y)
{
	this->mario->GetPosition(x, y);
}

void TestScene::FloatText(float x, float y)
{
	
	HUD* temp;

	if (combo < 9)
		combo += 1;

	y = y - FLOAT_TEXT_HEIGHT;

	switch (combo)
	{
	case 1:
		temp = new HUD(x, y, HUD_BONUS_POINT_100);
		global->point += HUD_BONUS_POINT_100;
		break;
	case 2:
		temp = new HUD(x, y, HUD_BONUS_POINT_200);
		global->point += HUD_BONUS_POINT_200;
		break;
	case 3:
		temp = new HUD(x, y, HUD_BONUS_POINT_400);
		global->point += HUD_BONUS_POINT_400;
		break;
	case 4:
		temp = new HUD(x, y, HUD_BONUS_POINT_800);
		global->point += HUD_BONUS_POINT_800;
		break;
	case 5:
		temp = new HUD(x, y, HUD_BONUS_POINT_1000);
		global->point += HUD_BONUS_POINT_1000;
		break;
	case 6:
		temp = new HUD(x, y, HUD_BONUS_POINT_2000);
		global->point += HUD_BONUS_POINT_2000;
		break;
	case 7:
		temp = new HUD(x, y, HUD_BONUS_POINT_4000);
		global->point += HUD_BONUS_POINT_4000;
		break;
	case 8:
		temp = new HUD(x, y, HUD_BONUS_POINT_8000);
		global->point += HUD_BONUS_POINT_8000;
		break;
	case 9:
		temp = new HUD(x, y, HUD_BONUS_POINT_UP);
		global->live += 1;
		break;
	default:
		break;
	}

	temp = NULL;

}

void TestScene::FloatTextCoin(float x, float y)
{
	global->point += 100;
	y = y - FLOAT_TEXT_HEIGHT;
	HUD* temp = new HUD(x, y, HUD_BONUS_POINT_100);
	temp->SetDrawOrder(HUD_TEXT_DRAW_ORDER);
}

void TestScene::FloatTextCustom(float x, float y, int point)
{
	global->live += 1;
	y = y - FLOAT_TEXT_HEIGHT;
	HUD* temp = new HUD(x, y, point);
	temp->SetDrawOrder(HUD_TEXT_DRAW_ORDER);
}

void TestScene::FloatEffectSplash(float x, float y)
{
	LPANIMATION ani = AnimationManager::GetInstance()->Get(BULLET)->Get(BULLET_EFFECT_SPLASH);
	HUD* temp = new HUD(x, y, ani);
}

void TestScene::SortGameObject()
{
	for (unsigned int j = 1; j < gameObjects.size(); ++j)
	{
		LPGAMEOBJECT key = gameObjects[j];
		int i = int(j) - 1;

		while (i >= 0 && gameObjects[i]->GetDrawOrder() > key->GetDrawOrder())
		{
			gameObjects[i + 1] = gameObjects[i];
			--i;
		}
		gameObjects[i + 1] = key;
	}
}

void TestScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	gameObjects.clear();
	collideObjects.clear();

	/*-----------------------Chia trong Grid----------------------*/

	for (int i = 0; i < numberOfCell; ++i)
	{
		LPGAMEOBJECT grid = new Grid(i, sceneFileBlock);
		cells.push_back(grid);
	}
	

	/*-----------------------Không chia trong Grid----------------------*/

	//Add Gate
	int currentScene, targetScene;
	LPGAMEOBJECT gate = NULL;
	float currentX, currentY;
	float targetX, targetY;
	int direction;
	int length = GATE.size();
	for (int x = 0; x < length; x += 7)
	{
		currentScene = GATE[x];
		currentX = GATE[x + 1] * STANDARD_SIZE;
		currentY = GATE[x + 2] * STANDARD_SIZE;
		targetScene = GATE[x + 3];
		targetX = GATE[x + 4] * STANDARD_SIZE;
		targetY = GATE[x + 5] * STANDARD_SIZE;
		direction = GATE[x + 6];

		if (currentX + currentY < 0)
		{
			currentWorld = currentScene;
			global->allowSwitch = false;
			continue;
		}		
		else if (targetX + targetY < 0)
			gate = new Teleport(currentScene, currentX, currentY, targetScene);
		else
			gate = new Teleport(currentScene, currentX, currentY, targetScene, targetX, targetY, direction);

		teleport.push_back(gate);
	}

	//Set Start Position
	if (allowResetStart)
	{
		startPosX = float(START[0]) * STANDARD_SIZE;
		startPosY = float(START[1]) * STANDARD_SIZE;
	}

	//Set Background color
	backgroundColor = D3DCOLOR_XRGB(COLOR[0], COLOR[1], COLOR[2]);

	//Setup HUD
	if (type != 2)
	{
		HUD* mainFrame = new HUD(eType::HUD_MAIN_FRAME);
		HUD* playerIcon = new HUD(eType::HUD_PLAYER_ICON);
		HUD* playerLive = new HUD(eType::HUD_PLAYER_LIVE);
		HUD* world = new HUD(eType::HUD_WORLD);
		HUD* speed = new HUD(eType::HUD_SPEEDOMETER);
		HUD* point = new HUD(eType::HUD_POINT);
		HUD* money = new HUD(eType::HUD_MONEY);
		HUD* time = new HUD(eType::HUD_TIME);
		HUD* cardOne = new HUD(eType::HUD_CARD_ONE);
		HUD* cardTwo = new HUD(eType::HUD_CARD_TWO);
		HUD* cardThree = new HUD(eType::HUD_CARD_THREE);
	}

	//Set screen width and height and camera mode
	global->screenWidth = this->width * STANDARD_SIZE;
	global->screenHeight = this->height * STANDARD_SIZE;
	global->cameraMode = SceneManager::GetInstance()->GetCurrentScene()->GetCameraMode();

	//Always start at cell 0
	static_cast<LPGRID>(cells[0])->Load(gameObjects, collideObjects);

	sort(gameObjects.begin(), gameObjects.end(), 
		[](const LPGAMEOBJECT& lhs, const LPGAMEOBJECT& rhs)
		{
			return lhs->GetDrawOrder() < rhs->GetDrawOrder();
		}
	);

	sort(collideObjects.begin(), collideObjects.end(),
		[](const LPGAMEOBJECT& lhs, const LPGAMEOBJECT& rhs)
		{
			return lhs->GetDrawOrder() > rhs->GetDrawOrder();
		}
	);

	PAUSE = false;
	Global::GetInstance()->background_color = backgroundColor;
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}


void TestScene::Update(DWORD dt)
{
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	//if (mario == NULL) return;


	if (startSwitch)
	{
		DWORD timePass = GetTickCount() - timeSwitch;

		if (startTimerSwitch)
		{
			GameEngine::GetInstance()->EnableColor();
			timeSwitch = GetTickCount();
			startTimerSwitch = false;
			PAUSE = true;
		}

		else if (timePass > SWITCH_TIME)
		{
			GameEngine::GetInstance()->SetColor(D3DCOLOR_XRGB(255, 255, 255));
			GameEngine::GetInstance()->DisableColor();
			startSwitch = false;
			startTimerSwitch = true;
			PAUSE = false;
		}
		else if (timePass > SWITCH_TIME * 3 / 4)
		{
			GameEngine::GetInstance()->SetColor(D3DCOLOR_XRGB(200, 200, 200));
		}
		else if (timePass > SWITCH_TIME * 1 / 2)
		{
			GameEngine::GetInstance()->SetColor(D3DCOLOR_XRGB(100, 100, 100));
		}
		else if (timePass > SWITCH_TIME * 1 / 4)
		{
			GameEngine::GetInstance()->SetColor(D3DCOLOR_XRGB(50, 50, 50));
		}
		else
		{
			GameEngine::GetInstance()->SetColor(D3DCOLOR_XRGB(0, 0, 0));
		}
	}

	for (auto x : gameObjects)
	{
		if (x != nullptr)
			x->Update(dt, &collideObjects);
	}

	
	//DebugOut(L"Size: %d\n", collideObjects.size());

	//Check cell
	if (type == 1)
		CheckCell();

	//float debugX, debugY;
	/*
	DebugOut(L"Cell: ");
	for (auto i : global->cells)
		DebugOut(L"%d ", i);
	DebugOut(L"\n");*/

	//Delete object
	for (auto deleteObject : deleteList)
	{
		//if (std::find(gameObjects.begin(), gameObjects.end(), deleteObject) == gameObjects.end())
			//DebugOut(L"Wrong!!!\n");

		//deleteObject->GetPosition(debugX, debugY);
		//DebugOut(L"x: %f -- y: %f\n", debugX, debugY);

		gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), deleteObject), gameObjects.end());
		delete deleteObject;
	}

	//Check if two list overlap
	for (auto deleteObject : deleteList)
	{
		addList.erase(std::remove(addList.begin(), addList.end(), deleteObject), addList.end());
	}

	//Add object
	for (auto addObject : addList)
	{
		gameObjects.push_back(addObject);
	}

	addList.clear();
	deleteList.clear();

	/*
	if (this->id == SCENE_WORLD_1_1)
	{
		DebugOut(L"here\n");
	}*/

	//Check teleport
	for (auto teleportObject : teleport)
	{
		if (static_cast<Teleport*>(teleportObject)->IsAllowSwitch())
		{
			global->allowSwitch = true;
			currentGate = teleportObject;
			break;
		}
		global->allowSwitch = false;
	}

	//Sort Game objects by draw order
	SortGameObject();

}

void TestScene::Render()
{
	for (auto x : gameObjects)
	{
		if (x != nullptr)
			x->Render();
	}
}

/*
	Unload current scene
*/
void TestScene::Unload()
{
	collideObjects.clear();
	teleport.clear();


	for (auto grid : cells)
	{
		LPGRID temp = static_cast<LPGRID>(grid);
		temp->Unload();
	}	

	for (auto deleteObject : deleteList)
	{
		gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), deleteObject), gameObjects.end());
		delete deleteObject;	
	}

	//Check if two list overlap
	for (auto deleteObject : deleteList)
	{
		addList.erase(std::remove(addList.begin(), addList.end(), deleteObject), addList.end());
	}

	//Delete newly added object
	for (auto addObject : addList)
	{
		delete addObject;
	}

	for (auto object : gameObjects)
		delete object;

	cells.clear();
	deleteList.clear();
	addList.clear();
}



