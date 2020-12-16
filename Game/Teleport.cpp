#include "Teleport.h"
#include "debug.h"

Teleport::Teleport(int targetScene) : GameObject()
{
	this->type = eType::TELEPORT_SCENE_TO_MAP;

	this->currentScene = -1;
	this->targetScene = targetScene;
	this->currentX = -1;
	this->currentY = -1;

	this->mario = NULL;

	global = Global::GetInstance();

	this->Add();

}

Teleport::Teleport(int currentScene, float x, float y, int targetScene) : GameObject()
{
	this->type = eType::TELEPORT_MAP;

	this->currentScene = currentScene;
	this->targetScene = targetScene;
	this->currentX = x + STANDARD_SIZE/2;
	this->currentY = y + STANDARD_SIZE/2;

	this->mario = NULL;

	global = Global::GetInstance();

	this->Add();
}

Teleport::Teleport(int currentScene, float x, float y, int targetScene, float tx, float ty, int direction) : GameObject()
{
	this->type = eType::TELEPORT_SCENE;

	this->currentScene = currentScene;
	this->targetScene = targetScene;
	this->currentX = x;
	this->currentY = y + 6.0f;
	this->targetX = tx;
	this->targetY = ty;

	this->mario = NULL;
	this->direction = direction;

	global = Global::GetInstance();

	this->Add();
}

void Teleport::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Add_Visual(this);

}

void Teleport::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Destroy_Visual(this);

}

void Teleport::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);

	if (type == eType::TELEPORT_SCENE_TO_MAP)
		return;

	if (mario == NULL)
	{
		LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
		LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

		if (current->GetMario() != NULL)
		{
			if (CHOOSE == 1)
				mario = static_cast<Mario*>(current->GetMario());
			else if (CHOOSE == 3)
				mario = static_cast<MarioMap*>(current->GetMario());
		}
	}
	else
	{
		mario->GetPosition(marioX, marioY);

		if (marioX + mario->GetWidth() >= currentX)
		{
			if (marioX <= currentX)
			{
				if (marioY + mario->GetHeight() >= currentY)
				{
					if (marioY <= currentY)
					{
						allowSwitch = true;
						return;
					}
				}		
			}	
		}

		//DebugOut(L"width: %d - height: %d\n", mario->GetWidth(), mario->GetHeight());
		//DebugOut(L"x: %f - y: %f\n", marioX, marioY);
		//DebugOut(L"tx: %f - ty: %f\n", currentX, currentY);
		allowSwitch = false;
	}
}
