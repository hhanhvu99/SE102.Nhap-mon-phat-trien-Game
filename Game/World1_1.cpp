#include "World1_1.h"
#include "debug.h"

World1_1::World1_1(int id, LPCWSTR filePath) : TestScene(id, filePath)
{
	this->type = 1;
}

void World1_1::Load()
{
	CHOOSE = 1;
	TestScene::Load();

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
	
	GameEngine::GetInstance()->UpdateCamPos(startPosX, startPosY);

	if (Global::GetInstance()->inWorldMap)
	{
		Global::GetInstance()->time = 300;
		Global::GetInstance()->inWorldMap = false;
	}
	
}

void World1_1::Update(DWORD dt)
{
	TestScene::Update(dt);

	//Check if touch ground
	
	if (castMario->isTouchGround())
		combo = 0;

	//soLanUpdate += 1;
	//DebugOut(L"CamX: %f - CamY: %f\n", cx, cy);

	if (castMario->isDying())
	{
		if (castMario->isReady())
			SetState(SCENE_STATE_STAGE_TO_MAP);
	}
	else if (castMario->isFinish())
	{
		if (castMario->isReady())
			SetState(SCENE_STATE_STAGE_TO_MAP);
	}
	else if (startSwitching)
	{
		if (castMario->isReady())
		{
			SetState(SCENE_STATE_SWITCH_NEXT);
		}
	}
	else if (stillSwitching)
	{
		if (castMario->isReady())
		{
			castMario->SetState(MARIO_STATE_TRANSPORT_RESET);
			stillSwitching = false;
		}
	}

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

void World1_1::Render()
{
	TestScene::Render();
}

void World1_1::Unload()
{
	TestScene::Unload();

}

void World1_1::SetState(int state)
{
	Scene::SetState(state);

	switch (state)
	{
	case SCENE_STATE_SWITCH:
		if (startSwitching == false)
		{
			Teleport* gate = static_cast<Teleport*>(currentGate);
			startSwitching = true;
				
			int direction;
			float cx, cy;
			float x, y;

			gate->GetDirection(direction);
			gate->GetCurrentPos(cx, cy);


			if (direction == 1 && GameEngine::GetInstance()->IsKeyDown(DIK_UP))
			{
				castMario->GetPosition(x, y);
				castMario->SetPosition(cx - 6.0f, y);
				castMario->SetTargetY(cy - castMario->GetHeight());
				castMario->SetState(MARIO_STATE_TRANSPORT_UP);
			}
			else if (direction == 2 && GameEngine::GetInstance()->IsKeyDown(DIK_DOWN))
			{
				castMario->GetPosition(x, y);
				castMario->SetPosition(cx - 6.0f, y);
				castMario->SetTargetY(cy + STANDARD_SIZE);
				castMario->SetState(MARIO_STATE_TRANSPORT_DOWN);
			}
			else if (direction == 3 && GameEngine::GetInstance()->IsKeyDown(DIK_DOWN))
			{
				castMario->GetPosition(x, y);
				castMario->SetPosition(cx - 6.0f, y);
				castMario->SetTargetY(cy + STANDARD_SIZE);
				castMario->SetState(MARIO_STATE_TRANSPORT_DOWN);
			}
				
		}
			
		break;
	case SCENE_STATE_SWITCH_NEXT:
	{
		Teleport* gate = static_cast<Teleport*>(currentGate);
		int direction;
		int height;
		float tx, ty;
		gate->GetDirection(direction);
		gate->GetTargetPos(tx, ty);
		height = castMario->GetHeight();

		PAUSE = true;

		SceneManager::GetInstance()->SwitchScene(gate->GetTargetID());
		LPTESTSCENE currentScene = static_cast<TestScene*>(SceneManager::GetInstance()->GetCurrentScene());

		//DebugOut(L"Direction: %d\n", direction);
		currentScene->DisableSwitch();
		currentScene->SetState(SCENE_STATE_STILL_SWITCH);
		castMario = static_cast<Mario*>(currentScene->GetMario());

		if (direction == 1)
		{
			castMario->SetPosition(tx - 6.0f, ty + height);
			castMario->SetTargetY(ty - (castMario->GetHeight() - STANDARD_SIZE));
			castMario->SetState(MARIO_STATE_TRANSPORT_UP);
		}
		else if (direction == 2)
		{
			castMario->SetPosition(tx - 6.0f, ty - height);
			castMario->SetTargetY(ty);
			castMario->SetState(MARIO_STATE_TRANSPORT_DOWN);
		}
		else if (direction == 3)
		{
			castMario->SetPosition(tx - 6.0f, ty + height);
			castMario->SetTargetY(ty - (castMario->GetHeight() - STANDARD_SIZE));
			castMario->SetState(MARIO_STATE_TRANSPORT_UP);
		}

	}
		break;
	case SCENE_STATE_STAGE_TO_MAP:
		SceneManager::GetInstance()->SwitchScene(currentWorld);
		break;
	case SCENE_STATE_STILL_SWITCH:
		stillSwitching = true;
		startSwitching = false;
		break;
	default:
		break;
	}
}
