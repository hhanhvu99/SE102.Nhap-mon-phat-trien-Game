#include "World.h"
#include "debug.h"

World::World(int id, LPCWSTR filePath) : TestScene(id, filePath)
{
	this->type = 1;
}

/*
	Load map play scene và menu
*/
void World::Load()
{
	CHOOSE = 1;
	TestScene::Load();

	//Thêm mario
	mario = new Mario(startPosX, startPosY);
	castMario = static_cast<Mario*>(mario);
	castMario->SetDrawOrder(PLAYER_DRAW_ORDER);
	castMario->SetLevel(global->level);

	if (global->level == MARIO_LEVEL_FROG)
		castMario->SetPosition(startPosX, startPosY - (MARIO_FROG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT));
	else if (global->level > MARIO_LEVEL_SMALL)
		castMario->SetPosition(startPosX, startPosY - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT));
	else
		castMario->SetPosition(startPosX, startPosY);

	Keyboard::GetInstance()->SetKeyHandler(castMario);
	castMario->SetAnimationSet(AnimationManager::GetInstance()->Get(MARIO));

	gameObjects.push_back(castMario);
	collideObjects.push_back(castMario);

	startSwitching = false;
	stillSwitching = false;
	global->allowCountTime = true;
	
	//Update vị trí camera
	GameEngine::GetInstance()->UpdateCamPos(startPosX, startPosY);

	//Nếu khi chuyển scene, scene lúc đầu là ở World Map thì reset thời gian chơi là 300 giây
	if (Global::GetInstance()->inWorldMap)
	{
		Global::GetInstance()->time = 300;
		Global::GetInstance()->inWorldMap = false;
	}
	
}

void World::Update(DWORD dt)
{
	TestScene::Update(dt);

	//Nếu chạm đất, combo reset bằng 0
	//Mỗi lần đạp một cái gì đó và thứ đó giúp mario nảy lên thì combo +1
	
	if (castMario->isTouchGround())
		combo = 0;

	//soLanUpdate += 1;
	//DebugOut(L"CamX: %f - CamY: %f\n", cx, cy);

	//Nếu mario chết
	if (castMario->isDying())
	{
		//Khi animation xong
		if (castMario->isReady())
			SetState(SCENE_STATE_STAGE_TO_MAP);
	}
	//Nếu mario chạm đích
	else if (castMario->isFinish())
	{
		//Khi chạy xong tới chữ cuối cùng
		if (castMario->isReady())
			SetState(SCENE_STATE_STAGE_TO_MAP);
	}
	//Nếu chuyển cảnh
	else if (startSwitching)
	{
		//Khi animation xong
		if (castMario->isReady())
		{
			SetState(SCENE_STATE_SWITCH_NEXT);
		}
	}
	//Sau khi chuyển từ cảnh cũ sang cảnh hiện tại, chạy thêm animation một lúc
	else if (stillSwitching)
	{
		//Chạy animation cho mario
		if (castMario->isReady())
		{
			castMario->SetState(MARIO_STATE_TRANSPORT_RESET);
			stillSwitching = false;
		}
	}

	//Đếm thời gian, theo dt, cứ qua 1000 là tính là một giây
	if (PAUSE == false && Global::GetInstance()->allowCountTime)
	{
		if (Global::GetInstance()->time > 0)
		{
			timeCounter += dt;

			if (timeCounter >= 1000)
			{
				Global::GetInstance()->time -= 1;
				timeCounter -= 1000;
			}
		}
	}
	
}

void World::Render()
{
	TestScene::Render();
}

void World::Unload()
{
	TestScene::Unload();

	//Nếu mario chết, reset toàn bộ map
	if (global->die)
	{
		for (int j = 0; j < this->height; ++j)
			for (int i = 0; i < this->width; ++i)
				activedBlock[j][i] = false;
	}

	if (this->id == SCENE_WORLD_1_1)
		SceneManager::GetInstance()->GetScene(SCENE_WORLD_1_1_BONUS)->ResetActivedBlock();
}

void World::SetState(int state)
{
	Scene::SetState(state);

	switch (state)
	{
		//Chuyển scene từ hiện tại
	case SCENE_STATE_SWITCH:
		if (startSwitching == false)
		{
			//Gate hiện tại để chuyển scene
			Teleport* gate = static_cast<Teleport*>(currentGate);
			startSwitching = true;
				
			int direction;
			float cx, cy;
			float x, y;

			gate->GetDirection(direction);
			gate->GetCurrentPos(cx, cy);

			//Ống cống trong play scene, di chuyển lên rồi lên
			if (direction == 1 && GameEngine::GetInstance()->IsKeyDown(DIK_UP))
			{
				castMario->GetPosition(x, y);
				castMario->SetPosition(cx - 6.0f, y);
				castMario->SetTargetY(cy - castMario->GetHeight());
				castMario->SetState(MARIO_STATE_TRANSPORT_UP);
			}
			//Ống cống trong play scene, di chuyển xuống rồi xuống
			else if (direction == 2 && GameEngine::GetInstance()->IsKeyDown(DIK_DOWN))
			{
				castMario->GetPosition(x, y);
				castMario->SetPosition(cx - 6.0f, y);
				castMario->SetTargetY(cy + STANDARD_SIZE);
				castMario->SetState(MARIO_STATE_TRANSPORT_DOWN);
			}
			//Ống cống trong play scene, di chuyển xuống rồi lên
			else if (direction == 3 && GameEngine::GetInstance()->IsKeyDown(DIK_DOWN))
			{
				castMario->GetPosition(x, y);
				castMario->SetPosition(cx - 6.0f, y);
				castMario->SetTargetY(cy + STANDARD_SIZE);
				castMario->SetState(MARIO_STATE_TRANSPORT_DOWN);
			}
				
		}
			
		break;
		//Chuyển scene tiếp theo
	case SCENE_STATE_SWITCH_NEXT:
	{
		//Gate hiện tại
		Teleport* gate = static_cast<Teleport*>(currentGate);
		int direction;
		int height;
		float tx, ty;
		gate->GetDirection(direction);
		gate->GetTargetPos(tx, ty);
		height = castMario->GetHeight();

		PAUSE = true;

		//Lấy scene tiếp theo
		SceneManager::GetInstance()->SwitchScene(gate->GetTargetID());
		LPTESTSCENE currentScene = static_cast<TestScene*>(SceneManager::GetInstance()->GetCurrentScene());

		//DebugOut(L"Direction: %d\n", direction);
		//Cài đặt scene mới cho tiếp tục animation 
		//Scene hiện tại chính là scene mới, code trong này vẫn chạy trong scene cũ
		currentScene->DisableSwitch();
		currentScene->SetState(SCENE_STATE_STILL_SWITCH);
		castMario = static_cast<Mario*>(currentScene->GetMario());

		//Lên thì lên
		if (direction == 1)
		{
			castMario->SetPosition(tx - 6.0f, ty + height);
			castMario->SetTargetY(ty - (castMario->GetHeight() - STANDARD_SIZE));
			castMario->SetState(MARIO_STATE_TRANSPORT_UP);
		}
		//Xuống thì xuống
		else if (direction == 2)
		{
			castMario->SetPosition(tx - 6.0f, ty - height);
			castMario->SetTargetY(ty);
			castMario->SetState(MARIO_STATE_TRANSPORT_DOWN);
		}
		//Xuống thì lên
		else if (direction == 3)
		{
			castMario->SetPosition(tx - 6.0f, ty + height);
			castMario->SetTargetY(ty - (castMario->GetHeight() - STANDARD_SIZE));
			castMario->SetState(MARIO_STATE_TRANSPORT_UP);
		}

	}
		break;
		//Chuyển từ play scene tới World Map
	case SCENE_STATE_STAGE_TO_MAP:
		SceneManager::GetInstance()->SwitchScene(currentWorld);
		break;
		//Scene vẫn còn trong trạng thái chuyển
	case SCENE_STATE_STILL_SWITCH:
		stillSwitching = true;
		startSwitching = false;
		break;
	default:
		break;
	}
}
