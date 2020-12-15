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
	castMario->SetPosition(startPosX, startPosY);

	Keyboard::GetInstance()->SetKeyHandler(castMario);
	castMario->SetAnimationSet(AnimationManager::GetInstance()->Get(MARIO));

	gameObjects.push_back(castMario);
	collideObjects.push_back(castMario);

	startSwitching = false;
	stillSwitching = false;

	float cx, cy;
	castMario->GetPosition(cx, cy);

	GameEngine* game = GameEngine::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	global->camY = (height - 12) * STANDARD_SIZE;

	GameEngine::GetInstance()->SetCamPos(cx, global->camY);

}

void World1_1::Update(DWORD dt)
{
	TestScene::Update(dt);

	//Check if touch ground
	
	if (castMario->isTouchGround())
		combo = 0;

	//soLanUpdate += 1;
	//DebugOut(L"Update lan thu: %d\n", soLanUpdate);

	if (castMario->isDying())
	{
		if (castMario->isReady())
		{
			SetState(SCENE_STATE_STAGE_TO_MAP);
		}
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
			castMario->GetPosition(x, y);

			castMario->SetPosition(cx - 6.0f, y);

			if (direction == 1)
				castMario->SetState(MARIO_STATE_TRANSPORT_UP);
			else if (direction == 2)
				castMario->SetState(MARIO_STATE_TRANSPORT_DOWN);
		}
			
		break;
	case SCENE_STATE_SWITCH_NEXT:
	{
		Teleport* gate = static_cast<Teleport*>(currentGate);
		int direction;
		float tx, ty;
		gate->GetDirection(direction);
		gate->GetTargetPos(tx, ty);

		PAUSE = true;

		SceneManager::GetInstance()->SwitchScene(gate->GetTargetID());
		LPTESTSCENE currentScene = static_cast<TestScene*>(SceneManager::GetInstance()->GetCurrentScene());

		//DebugOut(L"Direction: %d\n", direction);

		currentScene->SetState(SCENE_STATE_STILL_SWITCH);

		if (direction == 1)
		{
			currentScene->GetMario()->SetPosition(tx - 6.0f, ty + 16.0f);
			currentScene->GetMario()->SetState(MARIO_STATE_TRANSPORT_UP);
		}
		else if (direction == 2)
		{
			currentScene->GetMario()->SetPosition(tx - 6.0f, ty - 16.0f);
			currentScene->GetMario()->SetState(MARIO_STATE_TRANSPORT_DOWN);
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
