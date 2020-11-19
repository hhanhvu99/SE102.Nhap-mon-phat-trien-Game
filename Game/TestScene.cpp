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
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObject), gameObjects.end());
	collideObjects.erase(std::remove(collideObjects.begin(), collideObjects.end(), gameObject), collideObjects.end());

	delete gameObject;
}

void TestScene::Remove(LPGAMEOBJECT gameObject)
{
	collideObjects.erase(std::remove(collideObjects.begin(), collideObjects.end(), gameObject), collideObjects.end());
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
					gameObject = new QuestionBlock(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(QUESTION_BLOCK_ANI_HIT));
					gameObject->SetIndex(i, j);
					gameObject->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
					gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(ACTIVE_BLOCK));

					this->gameObjects.push_back(gameObject);
					this->collideObjects.push_back(gameObject);
					break;

				default:
					DebugOut(L"[ERROR] Cannot find the active block at loading scene. \n");
					break;
				}
				
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
	LPGAMEOBJECT enemy = NULL;

	length = ENEMY.size();
	for (int x = 0; x < length; x += 3)
	{
		placeX = ENEMY[x];
		placeY = ENEMY[x + 1];
		mobType = ENEMY[x + 2];
		switch (mobType)
		{
		case ENEMY_GROOMBA_BROWN:
			enemy = new Groomba(placeX, placeY, mobType);
			break;
		case ENEMY_KOOPAS_GREEN:
			enemy = new Koopas(placeX, placeY, mobType);
			break;
		default:
			DebugOut(L"[ERROR] Unknown mob type: %d\n", mobType);
		}
		
		enemy->SetDrawOrder(ENEMY_ENTITY_DRAW_ORDER);
		enemy->SetAnimationSet(AnimationManager::GetInstance()->Get(ENEMY_MOB));

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
	for (auto x : gameObjects)
	{
		x->Update(dt, &collideObjects);
	}
	

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (mario == NULL) return;

}

void TestScene::Render()
{
	for (auto x : gameObjects)
	{
		x->Render();
	}
}

/*
	Unload current scene
*/
void TestScene::Unload()
{
	
}


