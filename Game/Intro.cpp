#include "Intro.h"
#include "debug.h"

Intro::Intro(int id, LPCWSTR filePath) : TestScene(id, filePath)
{
	this->type = 2;
}

/*
	Kiểm tra object có rớt ra ngoài màn hình hay không
*/
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

void Intro::Load()
{
	CHOOSE = 2;
	TestScene::Load();
	Global::GetInstance()->background_color = D3DCOLOR_XRGB(0, 0, 0);

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
	allowCreateTurtleShell = false;

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

	firstOption = true;

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
		else if (i == 6 && j == 9)
		{
			arrow = static_cast<BackGround*>(object);
			arrow->SetType(eType::MENU_TITLE);
			arrow->SetAnimationSet(AnimationManager::GetInstance()->Get(MENU));
			arrow->SetDrawOrder(MENU_DRAW_ORDER_TITLE);
		}
		else if (i == 7 && j == 9)
		{
			text = static_cast<BackGround*>(object);
			text->SetType(eType::MENU_TITLE);
			text->SetAnimationSet(AnimationManager::GetInstance()->Get(MENU));
			text->SetDrawOrder(MENU_DRAW_ORDER_TITLE);
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

	//Chưa vẽ
	ribbon_top->SetDisableDraw();
	tree1->SetDisableDraw();
	tree2->SetDisableDraw();
	number->SetDisableDraw();
	arrow->SetDisableDraw();
	text->SetDisableDraw();
}

void Intro::Update(DWORD dt)
{
	TestScene::Update(dt);
	DWORD now = GetTickCount();
	
	//Kéo màn
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
			//Tạo mario và luigi
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
				luigi->SetType(eType::PLAYER_LUIGI);
				gameObjects.push_back(luigi);
				collideObjects.push_back(luigi);

				allowCreate = false;
			}
		}
			
	}
	//Chữ rớt xuống
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
					allowCreateTurtleShell = true;
				}
			}

			//Tạo các entity rớt xuống 
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
				groomba->SetState(ENEMY_STATE_DROP);

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
	//Chuyển màu background và các object gồm 4 giai đoạn
	else if (allowTranform)
	{
		if (startTranform)
		{
			tranformTime = now;
			startTranform = false;
		}

		DWORD timePass = now - tranformTime;
		D3DCOLOR color;

		//Giai đoạn 4
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
		//Giai đoạn 3
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
		//Giai đoạn 2
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
		//Giai đoạn 1
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

	//Di chuyển các entity
	if (allowEntity)
	{
		if (startEntity)
		{
			timeEntity = now;
			startEntity = false;
		}

		//Phần 1: Từ khi kéo màn tới lúc chuyển thành Raccoon
		if (allowPartOne)
		{
			DWORD timePass = now - timeEntity;

			//Mario đứng yên, luigi chạy ra ngoài màn hình
			if (timePass > MENU_TIME_ENTITY_THREE)
			{
				if (!mario->IsStop())
					mario->SetState(MARIO_MENU_STATE_IDLE);
				else
					allowPartOne = false;
			}
			//Luigi nhảy 1 lần
			else if (timePass > MENU_TIME_ENTITY_TWO)
			{
				if (stageTwoFirst)
				{
					luigi->SetState(MARIO_MENU_STATE_SHORT_JUMP);
					stageTwoFirst = false;
				}

			}
			//Luigi sang trái còn mario sang phải 
			else if (timePass > MENU_TIME_ENTITY_ONE)
			{
				mario->SetState(MARIO_MENU_STATE_WALKING_LEFT);
				luigi->SetState(MARIO_MENU_STATE_WALKING_RIGHT);
			}
			//Cả 2 đứng yên
			else
			{
				mario->SetState(MARIO_MENU_STATE_WALK_LEFT_NO_MOVE);
				luigi->SetState(MARIO_MENU_STATE_WALK_RIGHT_NO_MOVE);
			}
		}

		//Mario bị luigi dẫm lên
		if (mario->GetState() == MARIO_MENU_STATE_CROUCH)
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
				mario->SetState(MARIO_MENU_STATE_HIT_RELEASE);
				mario->SetState(MARIO_MENU_STATE_IDLE);

				
				moveTitle = true;
				setSpeed = true;

				waittingTime = now;
			}
		}
		//Mario bị vỏ rùa rơi trúng đầu
		else if (mario->IsHitByShell())
		{
			DWORD timePass = now - timeHitShell;

			//Nếu dính mai rùa vào đầu
			if (hitByShell)
			{
				timeHitShell = now;
				hitByShell = false;
			}
			//Nhảy cao ăn Super Leaf
			else if (timePass > MENU_MARIO_TITLE_4)
			{
				mario->SetState(MARIO_MENU_STATE_LONG_JUMP);
				mario->SetState(MARIO_MENU_STATE_HIT_RELEASE_2);
			}
			//Ngẩng đầu lên
			else if (timePass > MENU_MARIO_TITLE_3)
			{
				mario->SetState(MARIO_MENU_STATE_TITLE_2);
			}
			//Bình thường
			else if (timePass > MENU_MARIO_TITLE_2)
			{
				mario->SetState(MARIO_MENU_STATE_IDLE);
			}
			//Dính mai rùa
			else
			{
				mario->SetState(MARIO_MENU_STATE_TITLE_1);
			}

			//Luigi ra ngoài màn hình
			if (OutSideCam(luigi))
			{
				luigi->SetState(MARIO_MENU_STATE_IDLE);
				luigi->SetSpeed(0.0f, 0.0f);
				luigi->SetDirection(-1);
				luigi->SetType(eType::ENEMY_MOB_DIE);
			}
		}
		//Chuyển thành Raccoon, lúc này mario chưa chạm đất
		else if (mario->GetLevel() == MARIO_LEVEL_RACC && beginStageTwo == false)
		{
			//Nếu groomba chưa bị đạp
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

				//Đến giữa groomba rớt xuống
				if (mX <= gX + 2 * groomba->GetWidth())
				{
					mario->SetState(MARIO_MENU_STATE_STOP_JUMP);
				}
				//Mario rơi từ từ tới groomba
				else
				{
					mario->SetState(MARIO_MENU_STATE_JUMP_FLAP_HOLD);
					mario->SetState(MARIO_MENU_STATE_WALKING_LEFT);
				}
				//Groomba bị đạp
				if (groomba->GetState() == ENEMY_STATE_STOMP)
				{
					groomba = NULL;
					mario->SlowDown();
					inRaccoonFirst = true;
				}
			}

			//Nếu chạm đất, bắt đầu phần 2
			if (mario->isTouchGround())
			{
				beginStageTwo = true;
			}

		}
		//Phần 2: Từ khi chuyển thành Raccoon tới hết
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

			//Thắng lại lúc mario chạm đất
			if (allowBreak)
			{
				if (vx < 0)
				{
					turtleShell->SetGrapper(mario);
					mario->SetState(MARIO_MENU_STATE_BREAK_RIGHT);
				}
				else
				{
					allowPartTwo = true;
					allowBreak = false;
				}
			}

			if (allowPartTwo)
			{
				//Mario đứng yên sau khi nằm sau bụi cỏ
				if (timePass > MENU_MARIO_SECOND_16)
				{
					mario->SetState(MARIO_MENU_STATE_IDLE);
					showMenu = true;
					allowEntity = false;
				}
				//Mario chạy về bên phải, đồng thời đặt lại draw order
				else if (timePass > MENU_MARIO_SECOND_15)
				{
					mario->SetState(MARIO_MENU_STATE_RUNNING_RIGHT_CUS);
					mario->SetDrawOrder(MENU_DRAW_ORDER_RIBBON_BG);
				}
				//Mario đứng yên
				else if (timePass > MENU_MARIO_SECOND_14)
				{
					mario->SetState(MARIO_MENU_STATE_IDLE);
				}
				//Mario thắng rùi đi về bên trái
				else if (timePass > MENU_MARIO_SECOND_13)
				{
					if (vx > 0)
						mario->SetState(MARIO_MENU_STATE_BREAK_LEFT);
					else
						mario->SetState(MARIO_MENU_STATE_WALKING_LEFT);
				}
				//Mario chạy nhanh bên phải
				else if (timePass > MENU_MARIO_SECOND_12)
				{
					mario->SetState(MARIO_MENU_STATE_RUNNING_RIGHT_FAST);
				}
				//Mario đứng lại một khoảng
				else if (timePass > MENU_MARIO_SECOND_11)
				{
					mario->SetState(MARIO_MENU_STATE_IDLE);
				}
				//Mario sau khi bị dính rùa, mario quay mặt về bên trái một khoảng ngắn
				else if (timePass > MENU_MARIO_SECOND_10)
				{
					mario->SetState(MARIO_MENU_STATE_WALKING_LEFT);
				}
				//Đặt lại vị trí rùa ở rìa bên trái màn hình
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
				//Mario thả rùa và đứng yên, luigi chạy nhanh về bên phải
				else if (timePass > MENU_MARIO_SECOND_8)
				{
					mario->SetState(MARIO_MENU_STATE_RELEASE);
					mario->SetState(MARIO_MENU_STATE_IDLE);
					luigi->SetState(MARIO_MENU_STATE_RUNNING_RIGHT_FAST);
				}
				//Mario đi về bên phải, tay cầm rùa, luigi đứng yên
				else if (timePass > MENU_MARIO_SECOND_7)
				{
					mario->SetState(MARIO_MENU_STATE_HOLD);
					mario->SetState(MARIO_MENU_STATE_WALKING_RIGHT);
					luigi->SetState(MARIO_MENU_STATE_IDLE);
					luigiDisappear = true;
				}
				//Mario nhảy lên chạm mai rùa và đứng yên
				else if (timePass > MENU_MARIO_SECOND_6)
				{
					//Mario nhảy 1 lần
					if (jumpFirst)
					{
						mario->SetState(MARIO_MENU_STATE_VERY_SHORT_JUMP);
						jumpFirst = false;
					}
					//Đứng yên nếu chạm đất
					if (mario->isTouchGround())
					{
						mario->SetState(MARIO_MENU_STATE_IDLE);
					}
					//Nếu chạm mai rùa, đặt lại grapper của rùa là mario, quay mặt về bên phải
					else if (turtleShell->GetState() == ENEMY_STATE_STOMP)
					{
						mario->SetState(MARIO_MENU_STATE_IDLE);
						mario->SetDirection(1);
						turtleShell->SetGrapper(mario);
						turtleShell->SetRollSpeed(ENEMY_MENU_KOOPAS_ROLL_SPEED_SLOW);
					}


					luigi->SetState(MARIO_MENU_STATE_IDLE);
				}
				//Mario chạy về bên trái, luigi đá mai rùa
				else if (timePass > MENU_MARIO_SECOND_5)
				{
					mario->SetState(MARIO_MENU_STATE_WALKING_LEFT);
					luigi->SetState(MARIO_MENU_STATE_RELEASE);
					luigi->SetState(MARIO_MENU_STATE_IDLE);
					turtleShell->SetSpeed(-ENEMY_MENU_KOOPAS_ROLL_SPEED_CUSTOM, 0);
				}
				//Mario chạy về bên trái
				else if (timePass > MENU_MARIO_SECOND_4)
				{
					mario->SetState(MARIO_MENU_STATE_WALKING_LEFT);
					luigi->SetState(MARIO_MENU_STATE_HOLD);
					luigi->SetState(MARIO_MENU_STATE_IDLE);
				}
				//2 con đều đứng im
				else if (timePass > MENU_MARIO_SECOND_3)
				{
					mario->SetState(MARIO_MENU_STATE_IDLE);
					luigi->SetState(MARIO_MENU_STATE_HOLD);
					luigi->SetState(MARIO_MENU_STATE_IDLE);
				}
				//Luigi đi về bên trái, tay cầm con rùa
				else if (timePass > MENU_MARIO_SECOND_2)
				{
					mario->SetState(MARIO_MENU_STATE_IDLE);
					luigi->SetState(MARIO_MENU_STATE_HOLD);
					luigi->SetState(MARIO_MENU_STATE_WALKING_LEFT);
				}
				//Mario dừng, đặt lại vị trí luigi và cho luigi cầm con rùa
				else if (timePass > MENU_MARIO_SECOND_1)
				{
					mario->SetState(MARIO_MENU_STATE_IDLE);
					luigi->SetState(MARIO_MENU_STATE_HOLD);
					luigi->SetPosition(MENU_LUIGI_POSITION_X, MENU_LUIGI_POSITION_Y);
				}
				//Mario chạy sang phải, sau khi thắng xong
				else
				{
					mario->SetState(MARIO_MENU_STATE_WALKING_RIGHT);
				}

				//Nếu mai rùa ra khỏi màn hình
				if (OutSideCam(turtleShell))
				{
					//Lần đầu
					if (turtleShellFirst)
					{
						//Cho luigi cầm lên
						if (turtleShell->GetState() != ENEMY_STATE_BEING_GRAB)
						{
							luigi->SetType(eType::PLAYER);
							turtleShell->SetGrapper(luigi);
							turtleShell->SetState(ENEMY_STATE_BEING_GRAB);
							turtleShellFirst = false;

						}

						luigi->SetState(MARIO_MENU_STATE_HOLD);
					}
					//Lần hai, tự xoay sang bên phải
					else if (turtleShellSecond)
					{
						turtleShell->SetState(ENEMY_STATE_STOMP);
						turtleShellSecond = false;
					}

				}

				//Nếu luigi ra ngoài màn hình, cho luigi trở thành background
				if (luigiDisappear)
				{
					if (OutSideCam(luigi))
					{
						luigi->SetState(MARIO_MENU_STATE_IDLE);
						luigi->SetSpeed(0.0f, 0.0f);
						luigi->SetDirection(-1);
						luigi->SetType(eType::ENEMY_MOB_DIE);
					}

					luigiDisappear = false;
				}
			}

		}

	}
	
	//Menu
	if (showMenu)
	{
		if (menuFirst)
		{
			float x, y;
			float xNum, yNum;

			title->GetPosition(x, y);
			number->GetPosition(xNum, yNum);

			state = MENU_STATE_OPTION;

			moveRibbon = false;
			moveTitle = false;
			allowTranform = false;
			allowEntity = false;
			shaking = false;

			//Đặt lại scene menu, đồng thời hiện tất cả các prop bị giấu
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
	
			arrow->SetAllowDraw();
			text->SetAllowDraw();

			//Xóa hết mọi entity
			for (auto object : gameObjects)
			{
				if (object->GetType() != eType::MENU_TITLE && object->GetType() != eType::GROUP)
					Destroy(object);
			}

			waitingTime = now;
			currentTime = now;
			menuFirst = false;
		}

		//Spawn 4 con rùa khi menu hiện ra
		if (numberOfSpawn < 3)
		{
			//Canh thời gian spawn con rùa
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
		//Nếu rùa đủ 3 con, spawn con thứ 4
		else if (numberOfSpawn == 3)
		{
			float x, y;
			float cx, cy;
			firstTurtle->GetPosition(x, y);
			GameEngine::GetInstance()->GetCamPos(cx, cy);

			//Nếu con rùa đầu tiên chạm rìa bên phải
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

		//Arrow trên menu
		if (firstOption)
			arrow->SetPosition(MENU_ARROW_POS_1_X, MENU_ARROW_POS_1_Y);
		else
			arrow->SetPosition(MENU_ARROW_POS_2_X, MENU_ARROW_POS_2_Y);

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
	mario = NULL;
	luigi = NULL;

	ribbon = NULL;
	ribbon_top = NULL;
	title = NULL;
	tree1 = NULL;
	tree2 = NULL;
	number = NULL;
	arrow = NULL;
	text = NULL;

	groomba = NULL;
	turtleShell = NULL;
	firstTurtle = NULL;

	TestScene::Unload();
}

void Intro::SetState(int state)
{
	Scene::SetState(state);

	switch (state)
	{
	case SCENE_STATE_STAGE_TO_MAP:
		if (firstOption)
			SceneManager::GetInstance()->SwitchScene(SCENE_WORLD_1);

		break;
	case MENU_STATE_SKIP:
		showMenu = true;
		break;
	case MENU_STATE_OPTION:
		firstOption = !firstOption;
		break;
	default:
		break;
	}

}
