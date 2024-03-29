﻿#include <fstream>
#include "Grid.h"

#include "debug.h"

#define MAX_GAME_LINE 1024

Grid::Grid(int id, LPCWSTR fileBlock)
{
	this->id = id;
	this->pathToBlock = fileBlock;
	this->global = Global::GetInstance();
	this->Init();
}

/*
	LƯU Ý: Grid và Cell đều mang nghĩa tương đồng với nhau
*/

/*
	Khởi tạo object trong grid
*/
void Grid::Init()
{
	/*Load các object của Grid*/
	vector<int> CELL;

	ifstream f;
	f.open(pathToBlock);

	int option = 0;
	bool foundCell = false;
	char str[MAX_GAME_LINE];

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (foundCell) { break; }
		if (line[0] == '/') { continue; }

		if (line == "[CELL]") { option = 14; CELL.clear(); continue; }
		if (option == 14)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				CELL.push_back(atoi(tokens[i].c_str()));

			if (CELL[0] == this->id)
			{
				this->startCell_X = CELL[1] * STANDARD_SIZE;
				this->startCell_Y = CELL[2] * STANDARD_SIZE;
				this->endCell_X = CELL[3] * STANDARD_SIZE;
				this->endCell_Y = CELL[4] * STANDARD_SIZE;
				foundCell = true;
			}

			option = 0;
			continue;
		}

	}
	f.close();
}

/*
	Trong grid sẽ có các group bị overlap giữa 2 hoặc nhiều grid với nhau
	Tìm các GroupObject bị overlap khi thêm vào grid bằng cách so sánh vị trí
	Không so sánh bằng index là vì mỗi GameObject chỉ biết vị trí index tại điểm nó bắt đầu trên map 
	và GroupObject kế thừa từ GameObject nên ta không biết được điểm kết thúc nằm tại nếu tìm bằng index
*/
LPGAMEOBJECT Grid::FindGroup(int left, int top, int right, int bottom, vector<LPGAMEOBJECT>& collideObjects)
{
	//left, top, right, bottom là bounding box của group bị overlap, tương ứng với vị trí index trong map
	float leftPos = left * STANDARD_SIZE;
	float topPos = top * STANDARD_SIZE;
	float rightPos = (right + 1) * STANDARD_SIZE;
	float bottomPos = (bottom + 1) * STANDARD_SIZE;
	//objectLeft, objectTop, objectRight, objectBottom là GroupObject trùng 
	float objectLeft, objectTop;
	float objectRight, objectBottom;

	for (auto object : collideObjects)
	{
		//Nếu là GroupObject
		if (object->GetType() == eType::GROUP)
		{
			//Lấy vị trí group
			object->GetPosition(objectLeft, objectTop);
			objectRight = objectLeft + object->GetWidth();
			objectBottom = objectTop + object->GetHeight();

			//So sánh vị trí bounding box, nếu trùng 1 trong 2 góc: góc trái trên hoặc góc phải dưới thì trả về group
			if ((objectLeft == leftPos && objectTop == topPos) || (objectRight == rightPos && objectBottom == bottomPos))
				return object;
		}
	}

	return NULL;
}

/*
	Khi tìm được group bị overlap, ta kéo dài bounding box của GroupObject đang xét theo bounding box của group
	bằng index
*/
void Grid::FindGroupPos(int& left, int& top, int& right, int& bottom, LPGAMEOBJECT group)
{
	//Lấy index của group overlap
	int groupLeft, groupTop, groupRight, groupBottom;
	float x, y, width, height;

	group->GetPosition(x, y);
	width = float(group->GetWidth());
	height = float(group->GetHeight());

	groupLeft = int(x / STANDARD_SIZE);
	groupTop = int(y / STANDARD_SIZE);
	groupRight = int((x + width)/ STANDARD_SIZE) - 1;
	groupBottom = int((y + height)/ STANDARD_SIZE) - 1;

	//Tính chênh lệch giữa 4 cạnh của boundingbox
	int diffX_Left = groupLeft - left;
	int diffX_Right = right - groupRight;
	int diffY_Top = groupTop - top;
	int diffY_Bottom = bottom - groupBottom;

	//Chênh lệch có thể âm hoặc dương
	//Nếu left của cả 2 khác nhau, lấy right group hiện tại đang xét + cho chênh lệch
	if (diffX_Left != 0)
		right = left + diffX_Left - 1;
	//Nếu right của cả 2 khác nhau, lấy left group hiện tại đang xét + cho chênh lệch
	if (diffX_Right != 0)
		left = right - diffX_Right + 1;
	//Nếu top của cả 2 khác nhau, lấy bottom group hiện tại đang xét + cho chênh lệch
	if (diffY_Top != 0)
		bottom = top + diffY_Top - 1;
	//Nếu bottom của cả 2 khác nhau, lấy top group hiện tại đang xét + cho chênh lệch
	if (diffY_Bottom != 0)
		top = bottom - diffY_Bottom + 1;
	//Nếu không thay đổi, đặt index top, left, right, bottom sao cho thoát khỏi vòng for
	if (diffX_Left + diffX_Right + diffY_Top + diffY_Bottom == 0)
	{
		top = left = 1;
		right = bottom = 0;
	}

}

/*
	Thêm vào grid, đồng thời đặt lại currentCell 
*/
void Grid::Insert(LPGAMEOBJECT objectToAdd)
{
	cellObjects.push_back(objectToAdd);
	objectToAdd->SetCurrentCell(this->id);
}

/*
	Xóa object ra khỏi object holder của grid, không xóa object đó
*/
void Grid::Delete(LPGAMEOBJECT objectToRemove)
{
	cellObjects.erase(std::remove(cellObjects.begin(), cellObjects.end(), objectToRemove), cellObjects.end());
}

/*
	Load từ file vào
*/
void Grid::Load(vector<LPGAMEOBJECT>& gameObjects, vector<LPGAMEOBJECT>& collideObjects)
{
	/*Load các object của Grid*/
	//Tất cả các vector đều được đọc từ file ra
	vector<int> GROUP;
	vector<int> GROUP_MOVING;
	vector<int> PLATFORM;
	vector<int> ENEMY;
	vector<int> ITEM;
	vector<int> P_BLOCK_HOLDER;
	vector<int> CELL;

	unordered_map<int, vector<int>> GROUP_OVERLAP;

	ifstream f;
	f.open(pathToBlock);

	int currentGroup = 0;
	int option = 0;
	bool foundCell = false;

	char str[MAX_GAME_LINE];


	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '/') { continue; }

		if (line == "[CELL]") { option = 14; CELL.clear(); continue; }
		if (option == 14)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				CELL.push_back(atoi(tokens[i].c_str()));

			if (CELL[0] == this->id)
			{
				this->startCell_X = CELL[1] * STANDARD_SIZE;
				this->startCell_Y = CELL[2] * STANDARD_SIZE;
				this->endCell_X = CELL[3] * STANDARD_SIZE;
				this->endCell_Y = CELL[4] * STANDARD_SIZE;
				foundCell = true;
			}

			option = 0;
			continue;
		}

		if (foundCell == false)
			continue;

		if (line == "[GROUP]") { option = 3; continue; }
		if (line == "[ENEMY]") { option = 4; continue; }
		if (line == "[PLATFORM]") { option = 5; continue; }
		if (line == "[ITEM]") { option = 6; continue; }
		if (line == "[P-BLOCK]") { option = 11; continue; }
		if (line == "[GROUP_MOVING]") { option = 12; continue; }
		if (line == "[END]") { break; }
		if (option == 3)
		{
			vector<string> tokens = split(line, ",");
			vector<int> overlapCell;
			for (unsigned int i = 0; i < tokens.size(); ++i)
			{
				if (i < 4)
					GROUP.push_back(atoi(tokens[i].c_str()));
				else
					overlapCell.push_back(atoi(tokens[i].c_str()));
			}

			if (overlapCell.size() != 0)
				GROUP_OVERLAP[currentGroup] = overlapCell;

			currentGroup += 4;
				
		}
		else if (option == 4)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				ENEMY.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 5)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				PLATFORM.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 6)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				ITEM.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 11)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				P_BLOCK_HOLDER.push_back(atoi(tokens[i].c_str()));
		}
		else if (option == 12)
		{
			vector<string> tokens = split(line, ",");
			for (unsigned int i = 0; i < tokens.size(); ++i)
				GROUP_MOVING.push_back(atoi(tokens[i].c_str()));
		}
		else
		{
			DebugOut(L"[ERROR] Loading script: %s \n", pathToBlock);
		}


	}
	f.close();

	if (foundCell == false)
	{
		DebugOut(L"[ERROR] Loading script: %s \n", pathToBlock);
		return;
	}

	/*Load các object trong vùng của Grid*/
	int id;
	LPGAMEOBJECT gameObject, p_Block_Temp = NULL;
	LPSPRITEMANAGER sprites = SpriteManager::GetInstance();
	LPSCENE currentScene = SceneManager::GetInstance()->GetCurrentScene();

	//Nếu scene là World Map
	if (currentScene->GetType() == 3)
	{
		//Load các object trong map, mỗi id trong map tương ứng mỗi object
		for (int j = CELL[2]; j <= CELL[4]; ++j)
		{
			for (int i = CELL[1]; i <= CELL[3]; ++i)
			{
				id = currentScene->map[i][j];
				if (id == -1)
					continue;

				id += MAP_ID;

				gameObject = new BackGround(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(id));
				gameObject->SetIndex(i, j);
				gameObject->SetDrawOrder(MAP_DRAW_ORDER_BACKGROUND);
				gameObjects.push_back(gameObject);

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
	//Nếu scene không phải là World Map
	else
	{
		//Load các object trong map, mỗi id trong map tương ứng mỗi object
		for (int j = CELL[2]; j <= CELL[4]; ++j)
		{
			for (int i = CELL[1]; i <= CELL[3]; ++i)
			{
				id = currentScene->map[i][j];
				if (id == -1)
					continue;
				if (global->occupiedGroup.find(global->TwoDimension_To_OneDimension(i, j, currentScene->height)) != global->occupiedGroup.end())
					continue;
				if (find(begin(currentScene->GetBLOCKS()), end(currentScene->GetBLOCKS()), id) != end(currentScene->GetBLOCKS()))
				{
					gameObject = new Block(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(id));
					gameObject->SetIndex(i, j);
					gameObject->SetDrawOrder(BLOCK_DRAW_ORDER);

					gameObjects.push_back(gameObject);
					collideObjects.push_back(gameObject);
					cellObjects.push_back(gameObject);

				}
				else if (find(begin(currentScene->GetACTIVE_BLOCKS()), end(currentScene->GetACTIVE_BLOCKS()), id) != end(currentScene->GetACTIVE_BLOCKS()))
				{
					switch (id)
					{
					case BRICK_SHINY_ANI:
						//Nếu chưa bị phá
						if (currentScene->activedBlock[j][i] == false)
						{
							gameObject = new BrickShiny(STANDARD_SIZE * i, STANDARD_SIZE * j);
							gameObject->SetIndex(i, j);
							gameObject->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
							gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(ACTIVE_BLOCK));
							gameObject->SetCurrentCell(this->id);

							gameObjects.push_back(gameObject);
							collideObjects.push_back(gameObject);
							cellObjects.push_back(gameObject);
						}
						
						break;

					case QUESTION_BLOCK_ANI:
					{
						gameObject = new QuestionBlock(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(QUESTION_BLOCK_ANI_HIT));
						gameObject->SetIndex(i, j);
						gameObject->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
						gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(ACTIVE_BLOCK));

						//Nếu chưa bị đụng
						if (currentScene->activedBlock[j][i] == false)
						{
							Item* coin = new Coin(STANDARD_SIZE * i, STANDARD_SIZE * j, ITEM_COIN, false);
							coin->SetDrawOrder(BLOCK_DRAW_ORDER);
							coin->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
							coin->SetCurrentCell(this->id);

							static_cast<QuestionBlock*>(gameObject)->SetItem(coin);
						}
						else
							gameObject->SetState(QUESTION_BLOCK_STATE_DONE);
						

						gameObjects.push_back(gameObject);
						collideObjects.push_back(gameObject);
						cellObjects.push_back(gameObject);
					}

					break;

					case P_BLOCK_ANI:
					{
						gameObject = new P_Block(STANDARD_SIZE * i, STANDARD_SIZE * j);
						gameObject->SetIndex(i, j);
						gameObject->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
						gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(ACTIVE_BLOCK));

						//Nếu đã kích hoạt P_Block tới đoạn cuối cùng
						if (currentScene->activedBlock[j][i] == true)
							gameObject->SetState(P_BLOCK_STATE_DONE);

						gameObjects.push_back(gameObject);
						collideObjects.push_back(gameObject);
						cellObjects.push_back(gameObject);

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
					//Nếu chưa bị đụng
					if (currentScene->activedBlock[j][i] == false)
					{
						gameObject = new Coin(STANDARD_SIZE * i, STANDARD_SIZE * j, ITEM_COIN, true);
						gameObject->SetIndex(i, j);
						gameObject->SetDrawOrder(BLOCK_DRAW_ORDER);
						gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
						gameObject->SetCurrentCell(this->id);

						cellObjects.push_back(gameObject);
					}			

				}
				else if (id == END_GOAL_SPRITE_ID)
				{
					gameObject = new BackGround(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(END_GOAL_SQUARE));
					gameObject->SetIndex(i, j);
					gameObject->SetDrawOrder(BLOCK_DRAW_ORDER);
					gameObjects.push_back(gameObject);
					cellObjects.push_back(gameObject);

					gameObject = new EndGoal(STANDARD_SIZE * i, STANDARD_SIZE * j);
					gameObject->SetIndex(i, j);
					gameObject->SetDrawOrder(ACTIVE_BLOCK_DRAW_ORDER);
					gameObject->SetAnimationSet(AnimationManager::GetInstance()->Get(END_GOAL_ID));
					gameObjects.push_back(gameObject);
					collideObjects.push_back(gameObject);
					cellObjects.push_back(gameObject);
				}
				else
				{
					gameObject = new BackGround(STANDARD_SIZE * i, STANDARD_SIZE * j, sprites->Get(id));
					gameObject->SetIndex(i, j);
					gameObject->SetDrawOrder(BACKGROUND_DRAW_ORDER);
					gameObjects.push_back(gameObject);
					cellObjects.push_back(gameObject);
				}
			}
		}
	}

	//Thêm các object vào P_Block
	int objectX, objectY;
	int indexX, indexY;
	int length = P_BLOCK_HOLDER.size();
	P_Block* tempBlock = NULL;
	if (p_Block_Temp != NULL)
		tempBlock = static_cast<P_Block*>(p_Block_Temp);
	for (int x = 0; x < length; x += 2)
	{
		//Index x, y
		objectX = P_BLOCK_HOLDER[x];
		objectY = P_BLOCK_HOLDER[x + 1];

		//Nếu tại index đó brick đã bị phá
		if (currentScene->activedBlock[objectY][objectX] == true)
			continue;

		//DebugOut(L"Left: %d -- Top: %d -- Right: %d -- Bottom: %d \n", left,top,right,bottom);

		//Kiếm các object giống index và cho vào trong P_Block
		for (LPGAMEOBJECT object : gameObjects)
		{
			object->GetIndex(indexX, indexY);

			if (objectX == indexX && objectY == indexY)
			{
				tempBlock->AddObject(object);
				static_cast<BrickShiny*>(object)->SetMaster(tempBlock);
				break;
			}

		}

	}


	//Group object
	int left, top, right, bottom;
	bool foundDuplicate = false;
	GroupObject* group;
	vector<LPGAMEOBJECT>::iterator pos;
	vector<int> overlapCell;
	length = GROUP.size();
	for (int x = 0; x < length; x += 4)
	{
		//Kiểm tra có overlap không
		if (GROUP_OVERLAP.find(x) != GROUP_OVERLAP.end())
		{
			overlapCell = GROUP_OVERLAP[x];

			for (int cell : overlapCell)
			{
				if (global->cells.find(cell) != global->cells.end())
				{
					foundDuplicate = true;
					break;
				}
			}

		}

		left = GROUP[x];
		top = GROUP[x + 1];
		right = GROUP[x + 2];
		bottom = GROUP[x + 3];

		//Nếu không overlap
		if(foundDuplicate == false)
			group = new GroupObject();
		//Nếu có overlap, xác định object bị overlap và kéo dài bounding box của nó qua index left, top, right, bottom
		else
		{
			group = static_cast<GroupObject*>(FindGroup(left, top, right, bottom, collideObjects));
			FindGroupPos(left, top, right, bottom, group);
		}

		//DebugOut(L"Left: %d -- Top: %d -- Right: %d -- Bottom: %d \n", left,top,right,bottom);

		for (int j = top; j <= bottom; ++j)
			for (int i = left; i <= right; ++i)
			{
				for (LPGAMEOBJECT object : gameObjects)
				{
					object->GetIndex(indexX, indexY);

					if (i == indexX && j == indexY)
					{
						//Thêm object vào trong group
						group->Add(object);

						//Xóa object khỏi gameObjects, collideOjbects và cellObjects
						pos = find(gameObjects.begin(), gameObjects.end() - 1, object);
						gameObjects.erase(pos);
						gameObjects.shrink_to_fit();

						pos = find(cellObjects.begin(), cellObjects.end() - 1, object);
						cellObjects.erase(pos);
						cellObjects.shrink_to_fit();

						pos = find(collideObjects.begin(), collideObjects.end() - 1, object);
						collideObjects.erase(pos);
						collideObjects.shrink_to_fit();

						//DebugOut(L"Found\n");
						global->occupiedGroup.insert(global->TwoDimension_To_OneDimension(i, j, currentScene->height));

						break;
					}
				}
			}

		//Nếu không bị overlap
		if (foundDuplicate == false)
		{
			group->SetType(eType::GROUP);
			group->SetDrawOrder(BLOCK_DRAW_ORDER);
			group->SetOverLapCell(overlapCell);

			gameObjects.push_back(group);
			collideObjects.push_back(group);
			cellObjects.push_back(group);
		}
		else
		{
			foundDuplicate = false;
		}

	}

	//Group moving (platform di chuyển trong màn 4)
	length = GROUP_MOVING.size();
	for (int x = 0; x < length; x += 4)
	{
		left = GROUP_MOVING[x];
		top = GROUP_MOVING[x + 1];
		right = GROUP_MOVING[x + 2];
		bottom = GROUP_MOVING[x + 3];
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
						//Thêm object vào trong group
						group->Add(object);

						//Xóa object khỏi gameObjects, collideOjbects và cellObjects
						pos = find(gameObjects.begin(), gameObjects.end() - 1, object);
						gameObjects.erase(pos);
						gameObjects.shrink_to_fit();

						pos = find(cellObjects.begin(), cellObjects.end() - 1, object);
						cellObjects.erase(pos);
						cellObjects.shrink_to_fit();

						pos = find(collideObjects.begin(), collideObjects.end() - 1, object);
						collideObjects.erase(pos);
						collideObjects.shrink_to_fit();

						//DebugOut(L"Found\n");

						break;
					}
				}
			}

		group->SetType(eType::GROUP_MOVING);
		group->SetDrawOrder(ENEMY_ENTITY_DRAW_ORDER);
		gameObjects.push_back(group);
		collideObjects.push_back(group);
		cellObjects.push_back(group);
	}

	//Group platform object (platform nhảy lên được)
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
						//Thêm object vào trong group
						group->Add(object);

						//Xóa object khỏi gameObjects, collideOjbects và cellObjects
						pos = find(gameObjects.begin(), gameObjects.end() - 1, object);
						gameObjects.erase(pos);
						gameObjects.shrink_to_fit();

						pos = find(cellObjects.begin(), cellObjects.end() - 1, object);
						cellObjects.erase(pos);
						cellObjects.shrink_to_fit();

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
		cellObjects.push_back(group);
	}

	//Thêm Enemy
	int placeX;
	int placeY;
	int mobType;
	bool hasWing;
	LPGAMEOBJECT enemy = NULL;

	length = ENEMY.size();
	for (int x = 0; x < length; x += 4)
	{
		//Index x, y
		placeX = ENEMY[x];
		placeY = ENEMY[x + 1];
		//Kiểu enemy
		mobType = ENEMY[x + 2];
		//Có cánh không
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
		case ENEMY_BOOMERANG_BRO:
			enemy = new BoomerangBro(placeX, placeY, mobType);
			break;
		default:
			DebugOut(L"[ERROR] Unknown mob type: %d\n", mobType);
		}

		if (mobType >= ENEMY_PIRANHA_GREEN && mobType < ENEMY_BOOMERANG_BRO)
			enemy->SetDrawOrder(ENEMY_ENTITY_PLANT_DRAW_ORDER);
		else
			enemy->SetDrawOrder(ENEMY_ENTITY_DRAW_ORDER);

		//Nếu scene là World Map (dành cho ENEMY_TROOP)
		if (CHOOSE == 3)
		{
			enemy->SetAnimationSet(AnimationManager::GetInstance()->Get(MAP_ANI_ID));
			enemy->SetDrawOrder(MAP_DRAW_ORDER_ENEMY);
		}
		else
			enemy->SetAnimationSet(AnimationManager::GetInstance()->Get(ENEMY_MOB));

		//Đặt grid cho enemy
		enemy->SetCurrentCell(this->id);
		cellObjects.push_back(enemy);

	}


	//Thêm Item
	int itemType;
	Item* item = NULL;
	ActiveBlock* newObject;
	int indexObj_x, indexObj_y;
	bool isCoin = false;

	length = ITEM.size();
	for (int x = 0; x < length; x += 3)
	{
		//Index x, y
		indexX = ITEM[x];
		indexY = ITEM[x + 1];
		//Kiểu item
		itemType = ITEM[x + 2];

		//Nếu item tại index đó đã kích hoạt
		if (currentScene->activedBlock[indexY][indexX] == true)
			continue;

		switch (itemType)
		{
		case ITEM_MUSHROOM_RED:
		case ITEM_MUSHROOM_GREEN:
			item = new Mushroom(indexX * STANDARD_SIZE, indexY * STANDARD_SIZE, itemType);
			isCoin = false;
			break;
		case ITEM_SUPER_LEAF:
			item = new SuperLeaf(indexX * STANDARD_SIZE, indexY * STANDARD_SIZE, itemType);
			isCoin = false;
			break;
		case ITEM_SUPER_STAR:
			item = new SuperStar(indexX * STANDARD_SIZE, indexY * STANDARD_SIZE, itemType);
			isCoin = false;
			break;
		case ITEM_FIRE_FLOWER:
			item = new Flower(indexX * STANDARD_SIZE, indexY * STANDARD_SIZE, itemType);
			isCoin = false;
			break;
		case ITEM_COIN:
			isCoin = true;
			break;
		default:
			DebugOut(L"[ERROR] Unknown item type: %d\n", mobType);
		}

		//Thêm vào active block
		//Active block mặc định item là 1 coin
		for (auto object : collideObjects)
		{
			object->GetIndex(indexObj_x, indexObj_y);
			if (indexObj_x == indexX && indexObj_y == indexY)
			{
				//Active block có nhiều coin (đụng 7 lần)
				if (isCoin)
				{
					newObject = static_cast<ActiveBlock*>(object);
					newObject->SetOption(1);
					newObject->SetHP(7);
				}
				//Đặt item cho active block
				else
				{
					newObject = static_cast<ActiveBlock*>(object);

					if (newObject->hasItem())
						static_cast<LPTESTSCENE>(currentScene)->Destroy(newObject->GetItem());

					newObject->SetItem(item);
				}			
				break;
			}
		}
		
		//Không có 7 coin
		if (isCoin == false)
		{
			item->SetDrawOrder(BLOCK_DRAW_ORDER);
			item->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
			item->SetCurrentCell(this->id);

			cellObjects.push_back(item);
		}
		
	}


	//Thêm grid này vào global cells (các cells đang hoạt động)
	global->cells.insert(this->id);
}

void Grid::Unload()
{
	global->cells.erase(this->id);
	vector<LPGAMEOBJECT> listRemain;

	for (auto object : cellObjects)
	{
		//Là group
		if (object->GetType() == eType::GROUP)
			static_cast<GroupObject*>(object)->Destroy_Group();
		//Là các entity (enemy, item,...)
		else if (object->GetCurrentCell() != -1)
			listRemain.push_back(object);	
		else
			object->Destroy();
	}	

	//Phải để riêng để chặn việc xóa trong lúc lặp vector ở trên
	for (auto object : listRemain)
		object->Destroy();

	cellObjects.clear();

}

Grid::~Grid()
{
	cellObjects.clear();
}
