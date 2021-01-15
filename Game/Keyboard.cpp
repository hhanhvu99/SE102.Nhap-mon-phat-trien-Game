#include "Keyboard.h"
#include "Global.h"
#include "debug.h"

Keyboard* Keyboard::__instance = NULL;

void Keyboard::KeyState(BYTE* states)
{
	switch (CHOOSE)
	{
	case 1:
	{
		if (PAUSE == false)
		{
			GameEngine* game = GameEngine::GetInstance();

			float vx, vy;
			//int lastKeyPress = game->GetLastKey();
			mario->GetSpeed(vx, vy);

			//DebugOut(L"[INFO] Last key: %d\n", lastKeyPress);


			// disable control key when Mario die 
			if (mario->GetState() == MARIO_STATE_DIE) return;

			//Break
			if (mario->GetState() == MARIO_STATE_BREAK_RIGHT)
				if (vx != 0)
				{
					mario->SetState(MARIO_STATE_BREAK_RIGHT);
					return;
				}
			if (mario->GetState() == MARIO_STATE_BREAK_LEFT)
				if (vx != 0)
				{
					mario->SetState(MARIO_STATE_BREAK_LEFT);
					return;
				}

			//Attack
			if (game->IsKeyDown(DIK_A))
			{
				mario->SetState(MARIO_STATE_ATTACK);
				//mario->SetState(MARIO_STATE_IDLE);
				//return;
			}

			//Run
			if (mario->GetLevel() != MARIO_LEVEL_FROG)
			{
				if (game->IsKeyDown(DIK_Z))
				{
					if (game->IsKeyDown(DIK_RIGHT))
						if (mario->isCrouching())
							mario->SetState(MARIO_STATE_NOT_CROUCH);
						else
							if (vx < 0)
								mario->SetState(MARIO_STATE_BREAK_RIGHT);
							else
								mario->SetState(MARIO_STATE_RUNNING_RIGHT);
					else if (game->IsKeyDown(DIK_LEFT))
						if (mario->isCrouching())
							mario->SetState(MARIO_STATE_NOT_CROUCH);
						else
							if (vx > 0)
								mario->SetState(MARIO_STATE_BREAK_LEFT);
							else
								mario->SetState(MARIO_STATE_RUNNING_LEFT);
					else
						mario->SetState(MARIO_STATE_IDLE);
					return;
				}
			}

			//Walk
			if (game->IsKeyDown(DIK_RIGHT))
			{
				if (mario->isCrouching() && mario->GetLevel() != MARIO_LEVEL_FROG)
					mario->SetState(MARIO_STATE_NOT_CROUCH);
				else
					if (vx < 0)
						mario->SetState(MARIO_STATE_BREAK_RIGHT);
					else
						if (mario->GetLevel() != MARIO_LEVEL_FROG)
							mario->SetState(MARIO_STATE_WALKING_RIGHT);
						else
							mario->SetState(MARIO_STATE_WALKING_RIGHT_FROG);

				return;
			}
			else if (game->IsKeyDown(DIK_LEFT))
			{
				if (mario->isCrouching() && mario->GetLevel() != MARIO_LEVEL_FROG)
					mario->SetState(MARIO_STATE_NOT_CROUCH);
				else
					if (vx > 0)
						mario->SetState(MARIO_STATE_BREAK_LEFT);
					else
						if (mario->GetLevel() != MARIO_LEVEL_FROG)
							mario->SetState(MARIO_STATE_WALKING_LEFT);
						else
							mario->SetState(MARIO_STATE_WALKING_LEFT_FROG);

				return;
			}
			else
				mario->SetState(MARIO_STATE_IDLE);

			//Duck
			if (game->IsKeyDown(DIK_DOWN))
			{
				mario->SetState(MARIO_STATE_CROUCH);
				return;
			}
			else if (mario->isCrouching())
			{
				mario->SetState(MARIO_STATE_NOT_CROUCH);
			}
		}
	}
		break;
	default:
		break;
	}
	
	
}

void Keyboard::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	switch (CHOOSE)
	{
	case 1:
	{
		switch (KeyCode)
		{
		case DIK_Z:
			//Attack
			//Grabbing
			if (PAUSE == false)
			{
				mario->SetState(MARIO_STATE_ATTACK);
				mario->SetState(MARIO_STATE_HOLD);
			}
				
			break;
		case DIK_UP:
		case DIK_DOWN:
			if (PAUSE == false && Global::GetInstance()->allowSwitch == true)
				SceneManager::GetInstance()->GetCurrentScene()->SetState(SCENE_STATE_SWITCH);
			break;
		case DIK_S:
			//Short Jump
			if (PAUSE == false)
			{
				if (mario->isTouchGround() == false || mario->isJumpFlap())
					mario->SetState(MARIO_STATE_JUMP_FLAP);
				else
					mario->SetState(MARIO_STATE_SHORT_JUMP);
			}
			break;
		case DIK_X:
			//Jump high
			if (PAUSE == false)
			{
				if (mario->isTouchGround() == true)
					mario->SetState(MARIO_STATE_JUMP);
				else 
					mario->SetState(MARIO_STATE_JUMP_FLAP_HOLD);
				
			}
			break;
		//Debug
		case DIK_M:
			SceneManager::GetInstance()->GetCurrentScene()->SetState(SCENE_STATE_STAGE_TO_MAP);
			break;
		case DIK_Q:
			//Invincible
			if (PAUSE == false)
				mario->SetState(MARIO_STATE_INVINCIBLE);
			break;
		case DIK_I:
			//Transport Up
			mario->SetState(MARIO_STATE_TRANSPORT_UP);
			break;
		case DIK_K:
			//Transport Down
			mario->SetState(MARIO_STATE_TRANSPORT_DOWN);
			break;
		case DIK_P:
			//Flap Jumps
			if (PAUSE == false)
				mario->SetState(MARIO_STATE_DEBUG_FLAP);
			break;
		case DIK_H:
			mario->SetPosition(141 * STANDARD_SIZE, 5 * STANDARD_SIZE);
			mario->Reset();
			break;
		case DIK_1:
			mario->SetLevel(MARIO_LEVEL_SMALL);
			mario->Reset();
			break;
		case DIK_2:
			mario->SetLevel(MARIO_LEVEL_BIG);
			mario->Reset();
			break;
		case DIK_3:
			mario->SetLevel(MARIO_LEVEL_FIRE);
			mario->Reset();
			break;
		case DIK_4:
			mario->SetLevel(MARIO_LEVEL_RACC);
			mario->Reset();
			break;
		case DIK_5:
			mario->SetLevel(MARIO_LEVEL_FROG);
			mario->Reset();
			break;
		case DIK_6:
			mario->SetLevel(MARIO_LEVEL_TANU);
			mario->Reset();
			break;
		case DIK_7:
			mario->SetLevel(MARIO_LEVEL_HAM);
			mario->Reset();
			break;
		}
	}
		break;
	case 2:
	{
		switch (KeyCode)
		{
		case DIK_W:
			if (SceneManager::GetInstance()->GetCurrentScene()->GetState() == MENU_STATE_OPTION)
				SceneManager::GetInstance()->GetCurrentScene()->SetState(SCENE_STATE_STAGE_TO_MAP);
			else
				SceneManager::GetInstance()->GetCurrentScene()->SetState(MENU_STATE_SKIP);
			break;
		case DIK_Q:
			SceneManager::GetInstance()->GetCurrentScene()->SetState(MENU_STATE_OPTION);
			break;
		}
	}
		break;
	case 3:
	{
		switch (KeyCode)
		{
		case DIK_LEFTARROW:
			SceneManager::GetInstance()->GetCurrentScene()->SetState(MAP_STATE_MOVE_LEFT);
			break;
		case DIK_RIGHTARROW:
			SceneManager::GetInstance()->GetCurrentScene()->SetState(MAP_STATE_MOVE_RIGHT);
			break;
		case DIK_UPARROW:
			SceneManager::GetInstance()->GetCurrentScene()->SetState(MAP_STATE_MOVE_UP);
			break;
		case DIK_DOWNARROW:
			SceneManager::GetInstance()->GetCurrentScene()->SetState(MAP_STATE_MOVE_DOWN);
			break;
		case DIK_R:
			SceneManager::GetInstance()->GetCurrentScene()->SetState(MAP_STATE_RESET);
			break;
		case DIK_E:
			SceneManager::GetInstance()->GetCurrentScene()->SetState(MAP_STATE_RESTART);
			break;
		case DIK_Q:
			SceneManager::GetInstance()->GetCurrentScene()->SetState(MAP_STATE_SELECT);
			break;
		case DIK_W:
			if (Global::GetInstance()->allowSwitch && Global::GetInstance()->live > 0 && PAUSE == false)
				SceneManager::GetInstance()->GetCurrentScene()->SetState(SCENE_STATE_MAP_TO_STAGE);
			else
				SceneManager::GetInstance()->GetCurrentScene()->SetState(MAP_STATE_CHOOSE);
			
			break;
		}
	}
		break;
	default:
		break;
	}

}

void Keyboard::OnKeyUp(int KeyCode)
{
	switch (CHOOSE)
	{
	case 1:
	{
		switch (KeyCode)
		{
		case DIK_Z:
			if (PAUSE == false)
				mario->SetState(MARIO_STATE_RELEASE);
			break;
		case DIK_X:
			if (PAUSE == false)
				mario->SetState(MARIO_STATE_STOP_JUMP);
			break;
		}
	}
		break;
	default:
		break;
	}
	
}

Keyboard* Keyboard::GetInstance()
{
	if (__instance == NULL) __instance = new Keyboard();
	return __instance;
}
