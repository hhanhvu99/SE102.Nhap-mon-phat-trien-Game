#include "Intro.h"
#include "debug.h"

Intro::Intro(int id, LPCWSTR filePath) : TestScene(id, filePath)
{
}

bool Intro::OutSideCam(LPGAMEOBJECT object)
{
	float x, y;
	float cx, cy;
	object->GetPosition(x, y);
	GameEngine::GetInstance()->GetCamPos(cx, cy);

	if (x < cx - object->GetWidth() || x > cx + SCREEN_WIDTH || y < cy - object->GetHeight() || y > cy + SCREEN_HEIGHT)
		return true;

	return false;
}

void Intro::Reset()
{


}

void Intro::Load()
{
	TestScene::Load();

	isFirst = true;

	moveRibbon = true;
	setSpeed = true;

	moveTitle = false;
	shaking = false;
	startShaking = true;
	sign = 1;
	shakingTime = 0;
	waittingTime = 0;

	allowTranform = false;
	startTranform = true;
	tranformTime = 0;

	allowCreate = true;
	allowEntity = false;
	startEntity = true;
	timeEntity = 0;

	stageTwoFirst = true;
	allowCreateTurtleShell = true;

	allowPartOne = true;

	hitByShell = true;
	timeHitShell = 0;

	inRaccoonFirst = true;
	timeRaccoon = 0;

	beginStageTwo = false;
	allowPartTwo = false;
	allowBreak = true;
	turtleShellFirst = true;
	turtleShellSecond = true;
	luigiDisappear = false;
	jumpFirst = true;
	turtleRollFirst = true;
	turnFirst = true;

	showMenu = false;
	menuFirst = true;
	waitingTime = 0;

	numberOfSpawn = 0;
	currentTime = 0;

	Global::GetInstance()->background_color = D3DCOLOR_XRGB(0, 0, 0);
	AUTO = true;

	int i, j;
	float x, y;

	for (auto object : gameObjects)
	{
		object->GetIndex(i, j);

		if (i == 2 && j == 0)
		{
			ribbon_top = static_cast<BackGround*>(object);
			ribbon_top->SetType(eType::MENU_TITLE);
			ribbon_top->SetDrawOrder(MENU_DRAW_ORDER_RIBBON_BG);
		}
		else if (i == 2 && j == 1)
		{
			ribbon = static_cast<BackGround*>(object);
			ribbon->SetType(eType::MENU_TITLE);
			ribbon->GetPosition(x, y);
			ribbon->SetPosition(x, y - STANDARD_SIZE);
			ribbon->SetDrawOrder(MENU_DRAW_ORDER_RIBBON);
		}
		else if (i == 2 && j == 3)
		{
			title = static_cast<BackGround*>(object);
			title->SetType(eType::MENU_TITLE);
			title->GetPosition(x, y);
			title->SetPosition(x, y - STANDARD_SIZE * 12);
			title->SetDrawOrder(MENU_DRAW_ORDER_TITLE);
		}
		else if (i == 2 && j == 7)
		{
			tree1 = static_cast<BackGround*>(object);
			tree1->SetType(eType::MENU_TITLE);
			tree1->GetPosition(x, y);
			tree1->SetPosition(x, y + STANDARD_SIZE / 2);
			tree1->SetDrawOrder(MENU_DRAW_ORDER_TREE);
		}
		else if (i == 14 && j == 5)
		{
			tree2 = static_cast<BackGround*>(object);
			tree2->SetType(eType::MENU_TITLE);
			tree2->GetPosition(x, y);
			tree2->SetPosition(x, y + STANDARD_SIZE / 2);
			tree2->SetDrawOrder(MENU_DRAW_ORDER_TREE);
		}
		else if (i == 2 && j == 12)
		{
			object->SetType(eType::MENU_TITLE);
			object->GetPosition(x, y);
			object->SetPosition(x, y + STANDARD_SIZE / 2);
			object->SetDrawOrder(MENU_DRAW_ORDER_RIBBON_BG);
		}
		else if (i == 9 && j == 6)
		{
			number = static_cast<BackGround*>(object);
			number->SetType(eType::MENU_TITLE);
			number->SetAnimationSet(AnimationManager::GetInstance()->Get(MENU));
			number->GetPosition(x, y);
			number->SetPosition(x, y - STANDARD_SIZE * 10);
			number->SetDrawOrder(MENU_DRAW_ORDER_TITLE);
		}
		else
		{
			if (dynamic_cast<GroupObject*>(object))
			{
				GroupObject* group = static_cast<GroupObject*>(object);
				group->Move(0, STANDARD_SIZE / 2);
				group->SetDrawOrder(MENU_DRAW_ORDER_RIBBON_BG);
			}
		}
	}


	GameEngine::GetInstance()->SetCamPos(32.0f, 0.0f);

	ribbon_top->SetDisableDraw();
	tree1->SetDisableDraw();
	tree2->SetDisableDraw();
	number->SetDisableDraw();

}

void Intro::Update(DWORD dt)
{
	TestScene::Update(dt);
	DWORD now = GetTickCount();
	
	//Stage
	if (moveRibbon)
	{
		float x, y;
		ribbon->GetPosition(x, y);
	
		if (setSpeed)
		{
			ribbon->SetSpeed(0, -MENU_RIBBON_MOVE_Y);
			setSpeed = false;
		}

		if (y < -MENU_RIBBON_LIMIT)
		{
			moveRibbon = false;
			ribbon->SetSpeed(0, 0);


			allowEntity = true;
		}
		else if (y < -MENU_RIBBON_LIMIT / 2)
		{
			if (allowCreate)
			{
				mario = new PlayerIntro(272.0f, 152.0f);
				mario->SetAnimationSet(AnimationManager::GetInstance()->Get(MARIO));
				mario->SetLevel(MARIO_LEVEL_BIG);
				mario->SetDrawOrder(MENU_DRAW_ORDER_PLAYER);
				mario->SetDirection(-1);
				gameObjects.push_back(mario);
				collideObjects.push_back(mario);

				luigi = new PlayerIntro(32.0f, 152.0f);
				luigi->SetAnimationSet(AnimationManager::GetInstance()->Get(MARIO));
				luigi->SetLevel(LUIGI_LEVEL_BIG);
				luigi->SetDrawOrder(MENU_DRAW_ORDER_PLAYER);
				gameObjects.push_back(luigi);
				collideObjects.push_back(luigi);

				allowCreate = false;
			}
		}
			
	}
	else if (moveTitle)
	{
		DWORD timePass = now - waittingTime;

		if (timePass > MENU_TIME_TITLE_WAIT)
		{
			float x, y;
			float xNum, yNum;
			title->GetPosition(x, y);
			number->GetPosition(xNum, yNum);

			if (setSpeed)
			{
				title->SetSpeed(0, MENU_TITLE_MOVE_Y);
				number->SetSpeed(0, MENU_TITLE_MOVE_Y);
				setSpeed = false;
			}

			if (y > MENU_TITLE_Y && shaking == false)
			{
				title->SetSpeed(0, 0);
				number->SetSpeed(0, 0);
				shaking = true;
			}
			else if (shaking)
			{
				if (startShaking)
				{
					shakingTime = now;
					startShaking = false;
				}

				if (now - shakingTime > MENU_TITLE_MAX_TIME)
				{
					moveTitle = false;
					shaking = false;

					title->SetSpeed(0, 0);
					number->SetSpeed(0, 0);

					title->SetPosition(x, MENU_TITLE_Y);
					number->SetPosition(xNum, MENU_NUMBER_Y);

					allowTranform = true;
				}
			}

			if (allowCreateTurtleShell)
			{
				turtleShell = new KoopasIntro(9, -5, ENEMY_KOOPAS_MENU, true);
				turtleShell->SetDrawOrder(MENU_DRAW_ORDER_ENEMY);
				turtleShell->SetAnimationSet(AnimationManager::GetInstance()->Get(ENEMY_MOB));

				KoopasIntro* buzzyShell = new KoopasIntro(13, -3, ENEMY_BEETLE, true);
				buzzyShell->SetDrawOrder(MENU_DRAW_ORDER_ENEMY);
				buzzyShell->SetAnimationSet(AnimationManager::GetInstance()->Get(ENEMY_MOB));

				groomba = new Groomba(6, -2, ENEMY_GROOMBA_BROWN, false);
				groomba->SetDrawOrder(MENU_DRAW_ORDER_ENEMY);
				groomba->SetAnimationSet(AnimationManager::GetInstance()->Get(ENEMY_MOB));
				groomba->SetDirection(-1);
				groomba->SetState(ENEMY_STATE_IDLE);

				Mushroom* mushroom = new Mushroom(4 * STANDARD_SIZE, -1 * STANDARD_SIZE, ITEM_MUSHROOM_RED);
				mushroom->SetDrawOrder(MENU_DRAW_ORDER_ITEM);
				mushroom->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
				mushroom->SetDirection(-1);
				mushroom->SetState(ITEM_STATE_DROP);

				SuperLeaf* leaf = new SuperLeaf(10 * STANDARD_SIZE, -1 * STANDARD_SIZE, ITEM_SUPER_LEAF);
				leaf->SetDrawOrder(MENU_DRAW_ORDER_ITEM);
				leaf->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
				leaf->SetType(eType::ITEM);
				leaf->SetState(ITEM_STATE_MOVING);

				SuperStar* star = new SuperStar(15 * STANDARD_SIZE, -1 * STANDARD_SIZE, ITEM_SUPER_STAR);
				star->SetDrawOrder(MENU_DRAW_ORDER_ITEM);
				star->SetAnimationSet(AnimationManager::GetInstance()->Get(ITEM_ID));
				star->SetDirection(1);
				star->SetState(ITEM_STATE_DROP);

				allowCreateTurtleShell = false;
			}
		}
	
	}
	else if (allowTranform)
	{
		if (startTranform)
		{
			tranformTime = now;
			startTranform = false;
		}

		DWORD timePass = now - tranformTime;
		D3DCOLOR color;

		if (timePass > MENU_TIME_STAGE_FOUR)
		{
			color = D3DCOLOR_XRGB(255, 255, 255);
			Global::GetInstance()->background_color = D3DCOLOR_XRGB(252, 216, 168);
			allowTranform = false;

			title->SetDrawColor(color);
			tree1->SetDrawColor(color);
			tree2->SetDrawColor(color);
			number->SetDrawColor(color);
			ribbon_top->SetDrawColor(color);

			number->SetAllowDraw();
			allowTranform = false;

		}
		else if (timePass > MENU_TIME_STAGE_THREE)
		{
			color = D3DCOLOR_XRGB(200, 200, 200);
			Global::GetInstance()->background_color = D3DCOLOR_XRGB(252, 152, 56);

			title->SetDrawColor(color);
			tree1->SetDrawColor(color);
			tree2->SetDrawColor(color);
			number->SetDrawColor(color);
			ribbon_top->SetDrawColor(color);
		}
		else if (timePass > MENU_TIME_STAGE_TWO)
		{
			color = D3DCOLOR_XRGB(100, 100, 100);
			Global::GetInstance()->background_color = D3DCOLOR_XRGB(200, 76, 12);

			title->SetDrawColor(color);
			tree1->SetDrawColor(color);
			tree2->SetDrawColor(color);
			number->SetDrawColor(color);
			ribbon_top->SetDrawColor(color);
		}
		else if (timePass > MENU_TIME_STAGE_ONE)
		{
			color = D3DCOLOR_XRGB(50, 50, 50);
			Global::GetInstance()->background_color = D3DCOLOR_XRGB(124, 8, 0);

			ribbon_top->SetAllowDraw();
			tree1->SetAllowDraw();
			tree2->SetAllowDraw();

			title->SetSprite(SpriteManager::GetInstance()->Get(MENU_TITLE_TEXT_BRIGHT));

			title->SetDrawColor(color);
			tree1->SetDrawColor(color);
			tree2->SetDrawColor(color);
			number->SetDrawColor(color);
			ribbon_top->SetDrawColor(color);
		}

	}

	//Entity
	if (allowEntity)
	{
		if (startEntity)
		{
			timeEntity = now;
			startEntity = false;
		}

		if (allowPartOne)
		{
			DWORD timePass = now - timeEntity;

			if (timePass > MENU_TIME_ENTITY_THREE)
			{
				if (!mario->IsStop())
					mario->SetState(MARIO_STATE_IDLE);
				else
					allowPartOne = false;
			}
			else if (timePass > MENU_TIME_ENTITY_TWO)
			{
				if (stageTwoFirst)
				{
					luigi->SetState(MARIO_STATE_SHORT_JUMP);
					stageTwoFirst = false;
				}

			}
			else if (timePass > MENU_TIME_ENTITY_ONE)
			{
				mario->SetState(MARIO_STATE_WALKING_LEFT);
				luigi->SetState(MARIO_STATE_WALKING_RIGHT);
			}
			else
			{
				mario->SetState(MARIO_STATE_WALK_LEFT_NO_MOVE);
				luigi->SetState(MARIO_STATE_WALK_RIGHT_NO_MOVE);
			}
		}


		if (mario->GetState() == MARIO_STATE_CROUCH)
		{
			float x, y;
			float cx, cy;
			float vx, vy;
			GameEngine::GetInstance()->GetCamPos(cx, cy);
			luigi->GetPosition(x, y);
			luigi->GetSpeed(vx, vy);

			if (y < cy)
			{
				luigi->SetSpeed(vx, 0.2f);
				mario->SetState(MARIO_STATE_HIT_RELEASE);
				mario->SetState(MARIO_STATE_IDLE);

				
				moveTitle = true;
				setSpeed = true;

				waittingTime = now;
			}
		}
		else if (mario->IsHitByShell())
		{
			DWORD timePass = now - timeHitShell;

			if (hitByShell)
			{
				timeHitShell = now;
				hitByShell = false;
			}
			else if (timePass > MENU_MARIO_TITLE_4)
			{
				mario->SetState(MARIO_STATE_LONG_JUMP);
				mario->SetState(MARIO_STATE_HIT_RELEASE_2);
			}
			else if (timePass > MENU_MARIO_TITLE_3)
			{
				mario->SetState(MARIO_STATE_TITLE_2);
			}
			else if (timePass > MENU_MARIO_TITLE_2)
			{
				mario->SetState(MARIO_STATE_IDLE);
			}
			else
			{
				mario->SetState(MARIO_STATE_TITLE_1);
			}

			if (OutSideCam(luigi))
			{
				luigi->SetState(MARIO_STATE_IDLE);
				luigi->SetSpeed(0.0f, 0.0f);
				luigi->SetDirection(-1);
				luigi->SetType(eType::ENEMY_MOB_DIE);
			}
		}
		else if (beginStageTwo)
		{
			DWORD timePass = now - timeRaccoon;

			if (inRaccoonFirst)
			{
				timeRaccoon = now;
				inRaccoonFirst = false;
			}

			float vx, vy;
			mario->GetSpeed(vx, vy);

			if (allowBreak)
			{
				if (vx < 0)
				{
					turtleShell->SetGrapper(mario);
					mario->SetState(MARIO_STATE_BREAK_RIGHT);
				}
				else
				{
					allowPartTwo = true;
					allowBreak = false;
				}
			}


			if (allowPartTwo)
			{
				if (timePass > MENU_MARIO_SECOND_16)
				{
					mario->SetState(MARIO_STATE_IDLE);
					showMenu = true;
					allowEntity = false;
				}
				else if (timePass > MENU_MARIO_SECOND_15)
				{
					mario->SetState(MARIO_STATE_RUNNING_RIGHT_FAST);
				}
				else if (timePass > MENU_MARIO_SECOND_14)
				{
					mario->SetState(MARIO_STATE_IDLE);
					mario->SetDrawOrder(MENU_DRAW_ORDER_RIBBON_BG);
				}
				else if (timePass > MENU_MARIO_SECOND_13)
				{
					if (vx > 0)
						mario->SetState(MARIO_STATE_BREAK_LEFT);
					else
						mario->SetState(MARIO_STATE_WALKING_LEFT);
				}
				else if (timePass > MENU_MARIO_SECOND_12)
				{
					mario->SetState(MARIO_STATE_RUNNING_RIGHT_FAST);
				}
				else if (timePass > MENU_MARIO_SECOND_11)
				{
					mario->SetState(MARIO_STATE_IDLE);
				}
				else if (timePass > MENU_MARIO_SECOND_10)
				{
					mario->SetState(MARIO_STATE_WALKING_LEFT);
				}
				else if (timePass > MENU_MARIO_SECOND_9)
				{
					if (turtleRollFirst)
					{
						turtleShell->SetDirection(1);
						turtleShell->SetPosition(0.0f, 144.0f);
						turtleShell->SetState(ENEMY_STATE_ROLLING);
						turtleRollFirst = false;
					}

				}
				else if (timePass > MENU_MARIO_SECOND_8)
				{
					mario->SetState(MARIO_STATE_RELEASE);
					mario->SetState(MARIO_STATE_IDLE);
					luigi->SetState(MARIO_STATE_RUNNING_RIGHT_FAST);
				}
				else if (timePass > MENU_MARIO_SECOND_7)
				{
					mario->SetState(MARIO_STATE_HOLD);
					mario->SetState(MARIO_STATE_WALKING_RIGHT);
					luigi->SetState(MARIO_STATE_IDLE);
					luigiDisappear = true;
				}
				else if (timePass > MENU_MARIO_SECOND_6)
				{
					if (jumpFirst)
					{
						mario->SetState(MARIO_STATE_VERY_SHORT_JUMP);
						jumpFirst = false;
					}


					if (mario->isTouchGround())
					{
						mario->SetState(MARIO_STATE_IDLE);
					}
					else if (turtleShell->GetState() == ENEMY_STATE_STOMP)
					{
						mario->SetState(MARIO_STATE_IDLE);
						mario->SetDirection(1);
						turtleShell->SetGrapper(mario);
						turtleShell->SetRollSpeed(ENEMY_KOOPAS_ROLL_SPEED_SLOW);
					}


					luigi->SetState(MARIO_STATE_IDLE);
				}
				else if (timePass > MENU_MARIO_SECOND_5)
				{
					mario->SetState(MARIO_STATE_WALKING_LEFT);
					luigi->SetState(MARIO_STATE_RELEASE);
					luigi->SetState(MARIO_STATE_IDLE);
				}
				else if (timePass > MENU_MARIO_SECOND_4)
				{
					mario->SetState(MARIO_STATE_WALKING_LEFT);
					luigi->SetState(MARIO_STATE_HOLD);
					luigi->SetState(MARIO_STATE_IDLE);
				}
				else if (timePass > MENU_MARIO_SECOND_3)
				{
					mario->SetState(MARIO_STATE_IDLE);
					luigi->SetState(MARIO_STATE_HOLD);
					luigi->SetState(MARIO_STATE_IDLE);
				}
				else if (timePass > MENU_MARIO_SECOND_2)
				{
					mario->SetState(MARIO_STATE_IDLE);
					luigi->SetState(MARIO_STATE_HOLD);
					luigi->SetState(MARIO_STATE_WALKING_LEFT);
				}
				else if (timePass > MENU_MARIO_SECOND_1)
				{
					mario->SetState(MARIO_STATE_IDLE);
					luigi->SetState(MARIO_STATE_HOLD);
					luigi->SetPosition(MENU_LUIGI_POSITION_X, MENU_LUIGI_POSITION_Y);
				}
				else
				{
					mario->SetState(MARIO_STATE_WALKING_RIGHT);
				}

				if (OutSideCam(turtleShell))
				{
					if (turtleShellFirst)
					{
						if (turtleShell->GetState() != ENEMY_STATE_BEING_GRAB)
						{
							luigi->SetType(eType::PLAYER);
							turtleShell->SetGrapper(luigi);
							turtleShell->SetState(ENEMY_STATE_BEING_GRAB);
							turtleShellFirst = false;

						}

						luigi->SetState(MARIO_STATE_HOLD);
					}
					else if (turtleShellSecond)
					{
						turtleShell->SetState(ENEMY_STATE_STOMP);
						turtleShellSecond = false;
					}

				}

				if (luigiDisappear)
				{
					if (OutSideCam(luigi))
					{
						luigi->SetState(MARIO_STATE_IDLE);
						luigi->SetSpeed(0.0f, 0.0f);
						luigi->SetDirection(-1);
						luigi->SetType(eType::ENEMY_MOB_DIE);
					}

					luigiDisappear = false;
				}
			}

		}
		else if (mario->GetLevel() == MARIO_LEVEL_RACC)
		{
			if (groomba != NULL)
			{
				if (inRaccoonFirst)
				{
					groomba->SetState(ENEMY_STATE_MOVING);
					inRaccoonFirst = false;
				}


				float mX, mY;
				float gX, gY;

				groomba->GetPosition(gX, gY);
				mario->GetPosition(mX, mY);

				if (mX <= gX + 2 * groomba->GetWidth())
				{
					mario->SetState(MARIO_STATE_STOP_JUMP);
				}
				else
				{
					mario->SetState(MARIO_STATE_JUMP_FLAP_HOLD);
					mario->SetState(MARIO_STATE_WALKING_LEFT);
				}

				if (groomba->GetState() == ENEMY_STATE_STOMP)
				{
					groomba = NULL;
					mario->SlowDown();
					inRaccoonFirst = true;
				}
			}

			if (mario->isTouchGround())
			{
				beginStageTwo = true;
			}

		}

	}
	
	if (showMenu)
	{
		if (menuFirst)
		{
			float x, y;
			float xNum, yNum;

			title->GetPosition(x, y);
			number->GetPosition(xNum, yNum);

			moveRibbon = false;
			moveTitle = false;
			allowTranform = false;
			allowEntity = false;

			//Initiate final scene
			D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);
			Global::GetInstance()->background_color = D3DCOLOR_XRGB(252, 216, 168);

			ribbon->SetDisableDraw();

			title->SetSprite(SpriteManager::GetInstance()->Get(MENU_TITLE_TEXT_BRIGHT));
			title->SetSpeed(0, 0);
			title->SetPosition(x, MENU_TITLE_Y);
			title->SetDrawColor(color);

			number->SetAllowDraw();
			number->SetSpeed(0, 0);
			number->SetPosition(xNum, MENU_NUMBER_Y);
			number->SetDrawColor(color);

			ribbon_top->SetAllowDraw();
			ribbon_top->SetDrawColor(color);

			tree1->SetAllowDraw();
			tree1->SetDrawColor(color);

			tree2->SetAllowDraw();
			tree2->SetDrawColor(color);
	

			//Delete all entity
			for (auto object : gameObjects)
			{
				if (object->GetType() != eType::MENU_TITLE && object->GetType() != eType::GROUP)
					Destroy(object);
			}

			waitingTime = now;
			currentTime = now;
			menuFirst = false;
		}

		if (numberOfSpawn < 3)
		{
			if (now - currentTime > MENU_SPAWN_TIME)
			{
				KoopasIntro* koopas = new KoopasIntro(0, 9, ENEMY_KOOPAS_MENU, false);
				koopas->SetDrawOrder(MENU_DRAW_ORDER_ENEMY);
				koopas->SetAnimationSet(AnimationManager::GetInstance()->Get(ENEMY_MOB));
				koopas->SetDirection(1);
				++numberOfSpawn;
				currentTime = now;

				if (numberOfSpawn == 1)
					firstTurtle = koopas;
			}

		}
		else if (numberOfSpawn == 3)
		{
			float x, y;
			float cx, cy;
			firstTurtle->GetPosition(x, y);
			GameEngine::GetInstance()->GetCamPos(cx, cy);

			if (x + firstTurtle->GetWidth() > cy + SCREEN_WIDTH)
			{
				KoopasIntro* koopas = new KoopasIntro(0, 9, ENEMY_KOOPAS_MENU_SPECIAL, false);
				koopas->SetDrawOrder(MENU_DRAW_ORDER_ENEMY);
				koopas->SetAnimationSet(AnimationManager::GetInstance()->Get(ENEMY_MOB));
				koopas->SetDirection(1);
				koopas->IsSpecial();
				++numberOfSpawn;
			}
		}

		if (now - waitingTime > MENU_WAITING_TIME)
		{
			Unload();
			Load();
		}
		
	}
}

void Intro::Render()
{
	if (shaking)
	{
		sign = -sign;

		title->GetPosition(shakeX, shakeY);
		number->GetPosition(shakeXNum, shakeYNum);

		title->SetPosition(shakeX, shakeY + sign * MENU_TITLE_VIBRATION);
		number->SetPosition(shakeXNum, shakeYNum + sign * MENU_TITLE_VIBRATION);

	}

	//DebugOut(L"Render Size: %d\n", gameObjects.size());

	TestScene::Render();
}

void Intro::Unload()
{
	for (auto object : gameObjects)
		Destroy(object);

	mario = NULL;
	luigi = NULL;

	ribbon = NULL;
	ribbon_top = NULL;
	title = NULL;
	tree1 = NULL;
	tree2 = NULL;
	number = NULL;

	groomba = NULL;
	turtleShell = NULL;
	firstTurtle = NULL;

}

void Intro::SetState(int state)
{
	Scene::SetState(state);

	switch (state)
	{
	case MENU_STATE_SKIP:
		showMenu = true;
		break;
	default:
		break;
	}

}
