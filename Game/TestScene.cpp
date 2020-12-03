#include <iostream>
#include <fstream>
#include <algorithm>

#include "TestScene.h"
#include "debug.h"

TestScene::TestScene(int id, LPCWSTR filePath) : Scene(id, filePath)
{

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

void TestScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);
	f.close();

	int id;
	LPGAMEOBJECT gameObject;
	LPSPRITEMANAGER sprites = SpriteManager::GetInstance();
	mario = new Mario(45.0f, 350.0f);
	mario->SetDrawOrder(PLAYER_DRAW_ORDER);

	gameObjects.clear();
	collideObjects.clear();

	gameObjects.push_back(mario);
	collideObjects.push_back(mario);

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
					LPGAMEOBJECT coin = new Coin(STANDARD_SIZE * i, STANDARD_SIZE * j, ITEM_COIN, false);
					coin->SetDrawOrder(BLOCK_DRAW_ORDER);
					coin->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
					static_cast<QuestionBlock*>(gameObject)->SetItem(coin);

					this->gameObjects.push_back(gameObject);
					this->collideObjects.push_back(gameObject);
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

	int left, top, right, bottom;
	int indexX, indexY;
	GroupObject* group;
	vector<LPGAMEOBJECT>::iterator pos;

	//Group object
	int length = GROUP.size();
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
		default:
			DebugOut(L"[ERROR] Unknown mob type: %d\n", mobType);
		}
		
		if (mobType >= ENEMY_PIRANHA_GREEN)
			enemy->SetDrawOrder(ENEMY_ENTITY_PLANT_DRAW_ORDER);
		else
			enemy->SetDrawOrder(ENEMY_ENTITY_DRAW_ORDER);

		enemy->SetAnimationSet(AnimationManager::GetInstance()->Get(ENEMY_MOB));

	}

	//Add Item
	int itemType;
	LPGAMEOBJECT item = NULL;
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

	Keyboard::GetInstance()->SetKeyHandler(mario);
	mario->SetAnimationSet(AnimationManager::GetInstance()->Get(MARIO));
	mario->SetLevel(MARIO_LEVEL_SMALL);

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

	float cx, cy;
	mario->GetPosition(cx, cy);

	GameEngine* game = GameEngine::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	GameEngine::GetInstance()->SetCamPos(cx, 250.0f);

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}


void TestScene::Update(DWORD dt)
{
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (mario == NULL) return;

	for (auto x : gameObjects)
	{
		x->Update(dt, &collideObjects);
	}
	
	//Delete object
	for (auto deleteObject : deleteList)
	{
		gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), deleteObject), gameObjects.end());
		delete deleteObject;
	}
	deleteList.clear();

	//soLanUpdate += 1;
	//DebugOut(L"Update lan thu: %d\n", soLanUpdate);
}

void TestScene::Render()
{
	for (auto x : gameObjects)
	{
		if (x != NULL)
			x->Render();
	}
}

/*
	Unload current scene
*/
void TestScene::Unload()
{
	
}


