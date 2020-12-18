#include <iostream>
#include <fstream>
#include <algorithm>

#include "TestScene.h"
#include "debug.h"

TestScene::TestScene(int id, LPCWSTR filePath) : Scene(id, filePath)
{
	global = Global::GetInstance();
}

void TestScene::Add(LPGAMEOBJECT gameObject)
{
	gameObjects.push_back(gameObject);
	collideObjects.push_back(gameObject);

}

void TestScene::Destroy(LPGAMEOBJECT gameObject)
{
	auto objectDelete = std::find(collideObjects.begin(), collideObjects.end(), gameObject);
	if (objectDelete != collideObjects.end())
		collideObjects.erase(objectDelete);

	deleteList.push_back(gameObject);
}

void TestScene::Remove(LPGAMEOBJECT gameObject)
{
	auto objectDelete = std::find(collideObjects.begin(), collideObjects.end(), gameObject);
	if (objectDelete != collideObjects.end())
		collideObjects.erase(objectDelete);
}

void TestScene::Add_Visual(LPGAMEOBJECT gameObject)
{
	gameObjects.push_back(gameObject);
}

void TestScene::Destroy_Visual(LPGAMEOBJECT gameObject)
{
	deleteList.push_back(gameObject);
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
}

void TestScene::FloatTextCustom(float x, float y, int point)
{
	global->live += 1;
	y = y - FLOAT_TEXT_HEIGHT;
	HUD* temp = new HUD(x, y, point);
}

void TestScene::SortGameObject()
{
	for (int j = 1; j < gameObjects.size(); ++j)
	{
		LPGAMEOBJECT key = gameObjects[j];
		int i = j - 1;

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

	ifstream f;
	f.open(sceneFilePath);
	f.close();

	int id;
	LPGAMEOBJECT gameObject;
	LPSPRITEMANAGER sprites = SpriteManager::GetInstance();

	gameObjects.clear();
	collideObjects.clear();

	//gameObjects.push_back(mario);
	//collideObjects.push_back(mario);

	if (type == 3)
	{
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				id = map[i][j];
				if (id == -1)
					continue;

				id += MAP_ID;

				gameObject = new BackGround(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(id));
				gameObject->SetIndex(i, j);
				gameObject->SetDrawOrder(MAP_DRAW_ORDER_BACKGROUND);
				this->gameObjects.push_back(gameObject);

				if (id == MAP_TREE_ID)
				{
					gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(MAP_ANI_ID));
					gameObject->SetType(eType::MAP_TREE);
				}
				else if (id == MAP_POPUP_ID)
				{
					gameObject->SetDrawOrder(MAP_DRAW_ORDER_POPUP);
					gameObject->SetType(eType::MAP_POPUP);
				}
				
			}
		}
	}
	else
	{
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				id = map[i][j];
				if (id == -1)
					continue;
				if (find(begin(BLOCKS), end(BLOCKS), id) != end(BLOCKS))
				{
					gameObject = new Block(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(id));
					gameObject->SetIndex(i, j);
					gameObject->SetDrawOrder(BLOCK_DRAW_ORDER);
					this->gameObjects.push_back(gameObject);
					this->collideObjects.push_back(gameObject);

				}
				else if (find(begin(ACTIVE_BLOCKS), end(ACTIVE_BLOCKS), id) != end(ACTIVE_BLOCKS))
				{
					switch (id)
					{
					case BRICK_SHINY_ANI:
						gameObject = new BrickShiny(STANDARD_SIZE * i, STANDARD_SIZE * j);
						gameObject->SetIndex(i, j);
						gameObject->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
						gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(ACTIVE_BLOCK));

						this->gameObjects.push_back(gameObject);
						this->collideObjects.push_back(gameObject);
						break;

					case QUESTION_BLOCK_ANI:
					{
						gameObject = new QuestionBlock(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(QUESTION_BLOCK_ANI_HIT));
						gameObject->SetIndex(i, j);
						gameObject->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
						gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(ACTIVE_BLOCK));
						Item* coin = new Coin(STANDARD_SIZE * i, STANDARD_SIZE * j, ITEM_COIN, false);
						coin->SetDrawOrder(BLOCK_DRAW_ORDER);
						coin->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
						static_cast<QuestionBlock*>(gameObject)->SetItem(coin);

						this->gameObjects.push_back(gameObject);
						this->collideObjects.push_back(gameObject);
					}

					break;
					
					case P_BLOCK_ANI:
					{
						gameObject = new P_Block(STANDARD_SIZE * i, STANDARD_SIZE * j);
						gameObject->SetIndex(i, j);
						gameObject->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
						gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(ACTIVE_BLOCK));

						this->gameObjects.push_back(gameObject);
						this->collideObjects.push_back(gameObject);

						p_Block_Temp = gameObject;
					}
					break;

					default:
						DebugOut(L"[ERROR] Cannot find the active block at loading scene. \n");
						break;
					}

				}
				else if (id == ITEM_COIN_ID)
				{
					gameObject = new Coin(STANDARD_SIZE * i, STANDARD_SIZE * j, ITEM_COIN, true);
					gameObject->SetDrawOrder(BLOCK_DRAW_ORDER);
					gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));

				}
				else if (id == END_GOAL_SPRITE_ID)
				{
					gameObject = new BackGround(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(END_GOAL_SQUARE));
					gameObject->SetIndex(i, j);
					gameObject->SetDrawOrder(BLOCK_DRAW_ORDER);
					this->gameObjects.push_back(gameObject);

					gameObject = new EndGoal(STANDARD_SIZE * i, STANDARD_SIZE * j);
					gameObject->SetIndex(i, j);
					gameObject->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
					gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(END_GOAL_ID));
					this->gameObjects.push_back(gameObject);
					this->collideObjects.push_back(gameObject);
				}
				else
				{
					gameObject = new BackGround(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(id));
					gameObject->SetIndex(i, j);
					gameObject->SetDrawOrder(BACKGROUND_DRAW_ORDER);
					this->gameObjects.push_back(gameObject);
				}
			}
		}
	}
	

	//Add P_Block object
	int objectX, objectY;
	int indexX, indexY;
	int length = P_BLOCK_HOLDER.size();
	P_Block* tempBlock = static_cast<P_Block*>(p_Block_Temp);
	for (int x = 0; x < length; x += 2)
	{
		objectX = P_BLOCK_HOLDER[x];
		objectY = P_BLOCK_HOLDER[x + 1];

		//DebugOut(L"Left: %d -- Top: %d -- Right: %d -- Bottom: %d \n", left,top,right,bottom);
		
		for (LPGAMEOBJECT object : gameObjects)
		{
			object->GetIndex(indexX, indexY);

			if (objectX == indexX && objectY == indexY)
			{
				tempBlock->AddObject(object);
				static_cast<BrickShiny*>(object)->SetMaster(tempBlock);
			}	
		
		}
			
	}

	

	//Group object
	int left, top, right, bottom;
	GroupObject* group;
	vector<LPGAMEOBJECT>::iterator pos;
	length = GROUP.size();
	for (int x = 0; x < length; x+=4)
	{
		left = GROUP[x];
		top = GROUP[x + 1];
		right = GROUP[x + 2];
		bottom = GROUP[x + 3];
		group = new GroupObject();

		//DebugOut(L"Left: %d -- Top: %d -- Right: %d -- Bottom: %d \n", left,top,right,bottom);

		for(int j = top; j <= bottom; ++j)
			for (int i = left; i <= right; ++i)
			{
				for (LPGAMEOBJECT object : gameObjects)
				{
					object->GetIndex(indexX, indexY);
					
					if (i == indexX && j == indexY)
					{
						group->Add(object);

						pos = find(gameObjects.begin(), gameObjects.end()-1, object);
						gameObjects.erase(pos);
						gameObjects.shrink_to_fit();

						pos = find(collideObjects.begin(), collideObjects.end()-1, object);
						collideObjects.erase(pos);
						collideObjects.shrink_to_fit();
						
						//DebugOut(L"Found\n");

						break;
					}
				}
			}
		group->SetType(eType::GROUP);
		group->SetDrawOrder(BLOCK_DRAW_ORDER);
		gameObjects.push_back(group);
		collideObjects.push_back(group);
	}

	//Group platform object
	length = PLATFORM.size();
	for (int x = 0; x < length; x += 4)
	{
		left = PLATFORM[x];
		top = PLATFORM[x + 1];
		right = PLATFORM[x + 2];
		bottom = PLATFORM[x + 3];
		group = new GroupObject();

		//DebugOut(L"Left: %d -- Top: %d -- Right: %d -- Bottom: %d \n", left,top,right,bottom);

		for (int j = top; j <= bottom; ++j)
			for (int i = left; i <= right; ++i)
			{
				for (LPGAMEOBJECT object : gameObjects)
				{
					object->GetIndex(indexX, indexY);

					if (i == indexX && j == indexY)
					{
						group->Add(object);

						pos = find(gameObjects.begin(), gameObjects.end() - 1, object);
						gameObjects.erase(pos);
						gameObjects.shrink_to_fit();

						pos = find(collideObjects.begin(), collideObjects.end() - 1, object);
						collideObjects.erase(pos);
						collideObjects.shrink_to_fit();

						//DebugOut(L"Found\n");

						break;
					}
				}
			}
		group->SetType(eType::PLATFORM);
		group->SetDrawOrder(BLOCK_DRAW_ORDER);
		gameObjects.push_back(group);
		collideObjects.push_back(group);
	}

	//Add Enemy Mob
	int placeX;
	int placeY;
	int mobType;
	bool hasWing;
	LPGAMEOBJECT enemy = NULL;

	length = ENEMY.size();
	for (int x = 0; x < length; x += 4)
	{
		placeX = ENEMY[x];
		placeY = ENEMY[x + 1];
		mobType = ENEMY[x + 2];
		hasWing = ENEMY[x + 3];

		switch (mobType)
		{
		case ENEMY_GROOMBA_BROWN:
		case ENEMY_GROOMBA_RED:
			enemy = new Groomba(placeX, placeY, mobType, hasWing);
			break;
		case ENEMY_KOOPAS_GREEN:
		case ENEMY_KOOPAS_RED:
			enemy = new Koopas(placeX, placeY, mobType, hasWing);
			break;
		case ENEMY_PIRANHA_GREEN:
		case ENEMY_VENUS_GREEN:
		case ENEMY_VENUS_RED:
			enemy = new Plant(placeX, placeY, mobType);
			break;
		case ENEMY_TROOP:
			enemy = new EnemyTroop(placeX, placeY, mobType);
			break;
		default:
			DebugOut(L"[ERROR] Unknown mob type: %d\n", mobType);
		}
		
		if (mobType >= ENEMY_PIRANHA_GREEN)
			enemy->SetDrawOrder(ENEMY_ENTITY_PLANT_DRAW_ORDER);
		else
			enemy->SetDrawOrder(ENEMY_ENTITY_DRAW_ORDER);

		if (CHOOSE == 3)
		{
			enemy->SetAnimationSet(AnimationManager::GetInstance()->Get(MAP_ANI_ID));
			enemy->SetDrawOrder(MAP_DRAW_ORDER_ENEMY);
		}	
		else
			enemy->SetAnimationSet(AnimationManager::GetInstance()->Get(ENEMY_MOB));

	}

	//Add Item
	int itemType;
	Item* item = NULL;
	ActiveBlock* newObject;
	int indexObj_x, indexObj_y;

	length = ITEM.size();
	for (int x = 0; x < length; x += 3)
	{
		indexX = ITEM[x];
		indexY = ITEM[x + 1];
		itemType = ITEM[x + 2];

		switch (itemType)
		{
		case ITEM_MUSHROOM_RED:
		case ITEM_MUSHROOM_GREEN:
			item = new Mushroom(indexX * STANDARD_SIZE, indexY * STANDARD_SIZE, itemType);
			break;
		case ITEM_SUPER_LEAF:
			item = new SuperLeaf(indexX * STANDARD_SIZE, indexY * STANDARD_SIZE, itemType);
			break;
		case ITEM_SUPER_STAR:
			item = new SuperStar(indexX * STANDARD_SIZE, indexY * STANDARD_SIZE, itemType);
			break;
		default:
			DebugOut(L"[ERROR] Unknown item type: %d\n", mobType);
		}

		for (auto object : collideObjects)
		{
			object->GetIndex(indexObj_x, indexObj_y);
			if (indexObj_x == indexX && indexObj_y == indexY)
			{
				newObject = static_cast<ActiveBlock*>(object);

				if (newObject->hasItem())
					Destroy_Visual(newObject->GetItem());

				newObject->SetItem(item);
			}
		}

		item->SetDrawOrder(BLOCK_DRAW_ORDER);
		item->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));

	}

	//Add Gate
	int currentScene, targetScene;
	LPGAMEOBJECT gate = NULL;
	float currentX, currentY;
	float targetX, targetY;
	int direction;
	length = GATE.size();
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
			continue;
		}		
		else if (targetX + targetY < 0)
			gate = new Teleport(currentScene, currentX, currentY, targetScene);
		else
			gate = new Teleport(currentScene, currentX, currentY, targetScene, targetX, targetY, direction);

		teleport.push_back(gate);
	}
	
	//Set Stage Finished
	length = STAGE_FINISHED.size();
	for (int x = 0; x < length; x += 2)
	{
		indexX = STAGE_FINISHED[x];
		indexY = STAGE_FINISHED[x+1];

		for (LPGAMEOBJECT object : gameObjects)
		{
			object->GetIndex(indexObj_x, indexObj_y);

			if (indexX == indexObj_x && indexY == indexObj_y)
				object->SetSprite(SpriteManager::GetInstance()->Get(MAP_MARIO_FINISHED));
			
		}
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

	//Set screen width and height
	global->screenWidth = this->width * STANDARD_SIZE;
	global->screenHeight = this->height * STANDARD_SIZE;

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
	
	//Delete object
	for (auto deleteObject : deleteList)
	{
		gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), deleteObject), gameObjects.end());
		delete deleteObject;

	}
	deleteList.clear();

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

	for (auto object : gameObjects)
		Destroy(object);

	for (auto deleteObject : deleteList)
	{
		gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), deleteObject), gameObjects.end());
		delete deleteObject;	
	}
	deleteList.clear();
}



