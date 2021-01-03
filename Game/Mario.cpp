#include <algorithm>
#include <assert.h>
#include "Debug.h"

#include "Mario.h"
#include "GameEngine.h"
#include "Global.h"


Mario::Mario(float x, float y) : GameObject()
{
	global = Global::GetInstance();
	untouchable = 0;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	this->type = eType::PLAYER;
	this->grabObject = NULL;
	this->directionGrab = 0;

	touchGround = false;
	ani_walk_time = MARIO_ANI_WALKING_TIME_DEFAULT;
	lengthPalette = sizeof(InvicibilityPalette) / sizeof(InvicibilityPalette[0]);
	indexPalette = 0;

	SetState(MARIO_STATE_IDLE);
	SetLevel(global->level);
	this->Reset();
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	if (transporting)
	{
		GameEngine::GetInstance()->UpdateCamPos(x, y);

		if (state == MARIO_STATE_TRANSPORT_UP && y <= targetY)
		{
			PAUSE = false;
			transporting = false;

			vx = 0;
			vy = 0;

			readyToSwitch = true;
			return;
		}
		else if (state == MARIO_STATE_TRANSPORT_DOWN && y >= targetY)
		{
			PAUSE = false;
			transporting = false;

			vx = 0;
			vy = 0;

			readyToSwitch = true;
			return;
		}
		else
		{
			x += dx;
			y += dy;
			return;
		}

	}
	else if (readyToSwitch)
	{
		return;
	}

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	if (vy > MARIO_MAX_FALLING_SPEED)
		vy = MARIO_MAX_FALLING_SPEED;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
	{
		if (startInvincible)
			CalcPotentialCollisions(coObjects, coEvents, { eType::MARIO_BULLET, eType::ENEMY_MOB_DIE });
		else
			CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY, eType::MARIO_BULLET, eType::ENEMY_MOB_DIE });
	}
		
	else
	{
		vx = 0;

		if (dying)
		{
			if (GetTickCount() - die_time <= MARIO_DIE_TIME)
				vy = 0;
			else
			{
				vy = -MARIO_DIE_DEFLECT_SPEED;
				dying = false;
			}
				
		}
		else if (GetTickCount() - die_time > MARIO_DIE_TIME_SECOND)
			readyToSwitch = true;
	}

	if (transporting == false && finished == false)
	{
		if (fallOutside == false && die == false)
		{
			if (isOutSideCam())
			{
				fallOutside = true;
				SetState(MARIO_STATE_DIE);
			}
		}
		
	}

	// reset untouchable timer if untouchable time has passed
	if (untouchable)
	{
		if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
		{
			untouchable_start = 0;
			untouchable = false;
			type = eType::PLAYER;
		}
		else
		{
			type = eType::PLAYER_UNTOUCHABLE;
		}
	}

	if (inTransition)
	{
		vx = 0;
		vy = 0;

		if (state == MARIO_STATE_TRANSITION_1)
		{
			if (GetTickCount() - startTransitionOne > MARIO_TRANSITION_1_TIME)
			{
				inTransition = false;
				SetAnimationSet(AnimationManager::GetInstance()->Get(MARIO));
				PAUSE = false;
				SetLevel(global->level);
				type = eType::PLAYER;
			}
		}
		else
		{
			if (GetTickCount() - startTransitionTwo > MARIO_TRANSITION_2_TIME)
			{
				inTransition = false;
				PAUSE = false;
				SetLevel(global->level);
				type = eType::PLAYER;
			}
		}
		
	}

	if (startInvincible)
	{
		if (GetTickCount() - invincible_start > MARIO_INVINCIBILITY_TIME)
		{
			invincible_start = 0;
			startInvincible = false;

			if (global->level != MARIO_LEVEL_SMALL && global->level != MARIO_LEVEL_FROG)
			{
				if (isCrouch)
				{
					this->width = MARIO_BIG_CROUCH_BBOX_WIDTH;
					this->height = MARIO_BIG_CROUCH_BBOX_HEIGHT;
				}
				else
				{
					this->width = MARIO_BIG_BBOX_WIDTH;
					this->height = MARIO_BIG_BBOX_HEIGHT;
				}

				this->offset_Draw_X = MARIO_OFFSET_BIG_DRAW_X;
				this->offset_Draw_Y = MARIO_OFFSET_BIG_DRAW_Y;
			}

		}
	}

	// No collision occured, proceed normally
	//DebugOut(L"Size: %d \n", coEvents.size());

	if (direction < 0)
	{
		pointX = this->x + direction * (MARIO_POINT_COLLIDE_OFFSET_X);
		pointX2 = pointX;
		pointTail_X = pointX - MARIO_POINT_TAIL_OFFSET_X;
	}
	else if (direction > 0)
	{
		pointX = this->x + width + direction * (MARIO_POINT_COLLIDE_OFFSET_X);
		pointX2 = pointX;
		pointTail_X = pointX + MARIO_POINT_TAIL_OFFSET_X;
	}
		
	pointY = this->y + MARIO_POINT_COLLIDE_OFFSET_Y;
	pointY2 = this->y + this->height - MARIO_POINT_COLLIDE_OFFSET_Y;
	pointTail_Y = this->y + MARIO_POINT_TAIL_OFFSET_Y;

	bool result = PointCollision(*coObjects, pointX, pointY);
	bool result2 = PointCollision(*coObjects, pointX2, pointY2);

	if (result + result2 == 0)
	{
		touchLeft = false;
		touchRight = false;
	}

	//DebugOut(L"Result: %d\n", result);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		
		if (++countOffGround > 3)
		{
			touchGround = false;
			if (startJumping == false)
				jump_allow = false;
		}
	}
	else
	{
		float min_tx, min_ty;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
			//x += nx*abs(rdx); 

		//DebugOut(L"dx: %f -- dy: %f -- min_tx: %f -- min_ty: %f\n", dx, dy, min_tx, min_ty);

		// block every object first!
		this->x += min_tx * dx + nx * 0.4f;
		this->y += min_ty * dy + ny * 0.4f;

		if (nx != 0)
		{
			++countTouch;

			if (countTouch >= 2)
			{
				if (nx > 0)
				{
					touchLeft = true;
					if (isCrouch) state = MARIO_STATE_CROUCH;
					else state = MARIO_STATE_IDLE;
					vx = 0;
				}
				else
				{
					touchRight = true;
					if (isCrouch) state = MARIO_STATE_CROUCH;
					else state = MARIO_STATE_IDLE;
					vx = 0;
				}

			}

		}
		else
			countTouch = 0;



		if (ny != 0)
		{
			vy = 0;

			if (ny < 0)
			{
				touchGround = true;
				flapping = false;
				flapAni = false;
				startJumping = false;
				jump_allow = true;
				countOffGround = 0;
			}
			else if (ny > 0)
			{
				jump_allow = false;
			}

		}

		//float magnitude = sqrt(vx * vx + vy * vy);
		//int sign = Global::Sign(vy);

		//DebugOut(L"mintx: %f -- minty: %f\n", min_tx, min_ty);
		//DebugOut(L"dx: %f -- dy: %f\n", dx, dy);
		//DebugOut(L"vx: %f -- vy: %f\n", vx, vy);
		//DebugOut(L"M: %f\n", magnitude);
		//DebugOut(L"nx: %f -- ny: %f -- Touch ground: %d\n", nx, ny, touchGround);
		//DebugOut(L"Touch: %d -- nx: %f\n", touchGround, nx);
		//DebugOut(L"Touch left: %d -- Touch right: %d\n", touchLeft, touchRight);

		//DebugOut(L"Count Touch: %d \n", countTouch);


		
		

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			LPGAMEOBJECT obj = e->obj;

			if (obj->GetType() == eType::ITEM)
			{
				
				Item* item = static_cast<Item*>(obj);
				
				switch (item->getItemType())
				{
				case ITEM_MUSHROOM_RED:
					oldLevel = global->level;
					global->level = MARIO_LEVEL_BIG;

					if (oldLevel == MARIO_LEVEL_SMALL)
						SetState(MARIO_STATE_UP);
					else
						global->level = oldLevel;

					Global::GetInstance()->point += 1000;
					item->Destroy();
					break;

				case ITEM_MUSHROOM_GREEN:
					Global::GetInstance()->point += 1000;
					item->Destroy();
					break;

				case ITEM_SUPER_STAR:
					SetState(MARIO_STATE_INVINCIBLE);
					Global::GetInstance()->point += 1000;
					item->Destroy();
					break;

				case ITEM_SUPER_LEAF:
					oldLevel = global->level;
					global->level = MARIO_LEVEL_RACC;

					if (oldLevel == MARIO_LEVEL_BIG)
						SetState(MARIO_STATE_UP);
					else
						global->level = oldLevel;

					Global::GetInstance()->point += 1000;
					item->Destroy();
					break;
				case ITEM_COIN:
					item->Destroy();
					break;

				default:
					break;
				}

				continue;
			}

			if (obj->GetType() == eType::GOAL)
			{
				obj->SetState(GOAL_STATE_HIT);
				SetState(MARIO_STATE_FINISH);
			}
			else if (obj->GetType() == eType::P_BLOCK)
			{
				if (obj->GetState() == P_BLOCK_STATE_NORMAL)
				{
					if (ny < 0)
					{
						obj->SetState(P_BLOCK_STATE_STOMP);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}	
			}
			else if (obj->GetType() == eType::GROUP_MOVING)
			{
				float tempX, tempY, tempVx;
				obj->GetPosition(tempX, tempY);

				if (obj->GetState() == GROUP_MOVING_STATE_STOMP)
				{
					obj->GetSpeed(tempVx, this->vy);
					//DebugOut(L"Inside: vx: %f -- vy:%f\n", vx, vy);
				}
				else if (this->y + height <= tempY && ny < 0)
				{
					obj->SetState(GROUP_MOVING_STATE_STOMP);
				}

			}
			if (ny > 0)
			{
				if (obj->GetType() == eType::BRICK || obj->GetType() == eType::QUESTION)
				{
					obj->SetState(ACTIVE_BLOCK_STATE_HIT);
				}
			}
			if (startInvincible)
			{
				if (obj->GetType() == eType::ENEMY)
				{
					obj->SetState(ENEMY_STATE_HIT);
					obj->SetDirection(direction);
				}
			}
			
				
		}

	}

	//DebugOut(L"Outside: vx: %f -- vy:%f\n", vx, vy);
	//DebugOut(L"x: %f -- y:%f\n", x, y);
	//DebugOut(L"Is holding: %d\n", grabTurtlePress);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
		coEvents[i] = NULL;
	}

	//Finish
	if (finished)
	{
		DWORD timePass = GetTickCount() - startFinish;

		if (touchGround)
		{
			direction = 1;
			vx = MARIO_WALKING_FINISH_SPEED;
			ani_walk_time = 50;
		}	

		if (finishWaiting)
		{
			if (timePass > MARIO_FINISH_TIME_4)
				readyToSwitch = true;
		}
		else 
		{
			if (timePass > MARIO_FINISH_TIME_3)
			{
				if (global->time > 0)
				{
					global->time -= 1;
					global->point += 50;
				}
				else
				{
					finishWaiting = true;
					startFinish = GetTickCount();
				}
			}
			else if (timePass > MARIO_FINISH_TIME_2)
			{
				if (firstText_2)
				{
					LPSPRITE sprite = SpriteManager::GetInstance()->Get(END_GOAL_TEXT_2);
					int id = HUD_ID;

					if (global->cardGet == END_GOAL_STAR)
						id += HUD_ITEM_STAR;
					else if (global->cardGet == END_GOAL_FLOWER)
						id += HUD_ITEM_FLOWER;
					else
						id += HUD_ITEM_MUSHROOM;

					LPSPRITE iconCard = SpriteManager::GetInstance()->Get(id);

					HUD* text2 = new HUD(global->frameHUD_x + HUD_TEXT_2_X, global->frameHUD_y + HUD_TEXT_2_Y, sprite);
					HUD* card = new HUD(global->frameHUD_x + HUD_ITEM_ICON_X, global->frameHUD_y + HUD_ITEM_ICON_Y, iconCard);
					firstText_2 = false;
				}
			}
			else if (timePass > MARIO_FINISH_TIME_1)
			{
				if (firstText_1)
				{
					LPSPRITE sprite = SpriteManager::GetInstance()->Get(END_GOAL_TEXT_1);
					HUD* text1 = new HUD(global->frameHUD_x + HUD_TEXT_1_X, global->frameHUD_y + HUD_TEXT_1_Y, sprite);
					firstText_1 = false;
				}

			}
			else
			{
				global->allowCountTime = false;
			}
		}
		
		

		return;
	}

	//Is running or not?
	if (abs(vx) > MARIO_MAX_WALKING_SPEED)
		isRunning = true;
	else
		isRunning = false;

	//Update animation time
	float speed = abs(vx);
	if (global->level != MARIO_LEVEL_FROG)
	{
		if (speed < MARIO_MAX_WALKING_SPEED)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_DEFAULT;
			global->speed = 0;
			if (touchGround)
				isMax = false;
		}
		else if (speed == MARIO_MAX_WALKING_SPEED)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_WARMUP;
			global->speed = 0;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_HALF_MAX_RUNNING_SPEED_1)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_WARMUP;
			global->speed = 1;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_HALF_MAX_RUNNING_SPEED_2)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_WARMUP;
			global->speed = 2;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_HALF_MAX_RUNNING_SPEED_3)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_WARMUP;
			global->speed = 3;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_MAX_RUNNING_SPEED_1)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_RUN;
			global->speed = 4;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_MAX_RUNNING_SPEED_2)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_RUN;
			global->speed = 5;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_MAX_RUNNING_SPEED_3)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_RUN;
			global->speed = 6;
			if (touchGround)
				isMax = false;
		}
		else
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_MAX;
			global->speed = 7;
		}
			
	}
	else
	{
		ani_walk_time = MARIO_ANI_FROG_JUMPING_TIME;
	}
	//DebugOut(L"Ani time: %d \n", ani_walk_time);

	//Invincibility BBox
	if (global->level != MARIO_LEVEL_SMALL && global->level != MARIO_LEVEL_FROG)
		if (startInvincible)
		{
			if (isCrouch)
			{
				this->width = MARIO_BIG_CROUCH_BBOX_WIDTH;
				this->height = MARIO_BIG_CROUCH_BBOX_HEIGHT;
				this->offset_Draw_X = MARIO_OFFSET_BIG_DRAW_X;
				this->offset_Draw_Y = MARIO_OFFSET_BIG_DRAW_Y;
			}
			else if (touchGround == false)
			{
				this->width = MARIO_BIG_ROLL_BBOX_WIDTH;
				this->height = MARIO_BIG_ROLL_BBOX_HEIGHT;
				this->offset_Draw_X = MARIO_OFFSET_INVINCIBLE_DRAW_X;
				this->offset_Draw_Y = MARIO_OFFSET_INVINCIBLE_DRAW_Y;
			}
			else
			{
				this->width = MARIO_BIG_BBOX_WIDTH;
				this->height = MARIO_BIG_BBOX_HEIGHT;
				this->offset_Draw_X = MARIO_OFFSET_BIG_DRAW_X;
				this->offset_Draw_Y = MARIO_OFFSET_BIG_DRAW_Y;
			}
		}

	//Frog 
	if (frog_jumping && touchGround && (touchLeft + touchRight == 0))
	{
		if (GetTickCount() - startFrogJump > MARIO_FROG_JUMPING_TIME)
		{
			frog_jumping = false;
			if (direction > 0) vx = 0.0005f;
			else if (direction < 0) vx = -0.0005f;
		}
	}

	//Jumping
	if (jump_allow)
	{
		if (GetTickCount() - jump_start < MARIO_MAX_JUMPING)
			vy -= (MARIO_JUMPING_SPEED + MARIO_GRAVITY) * dt;
		else
			jump_allow = false;
	}

	//Attack
	if (ani != -1)
	{
		int attackLeft = global->level + MARIO_ANI_ATTACK_SET_LEFT;
		int attackRight = global->level + MARIO_ANI_ATTACK_SET_RIGHT;

		if (ani == attackLeft || ani == attackRight)
		{
			size = animation_set->Get(ani)->GetSize();
			currentFrame = animation_set->Get(ani)->GetCurrentFrame();
			//DebugOut(L"Frame: %d \n", currentFrame);
			//DebugOut(L"Flip: %d \n", flipFrame);
			//DebugOut(L"Size: %d \n", size);
			//DebugOut(L"Ani: %d \n", ani);


			if (flipFrame)
			{
				if (currentFrame != size - 1)
				{
					if (tail_whip) tail_whip = false;
					else shoot = false;
					flipFrame = false;
				}
			}

			if (currentFrame == size - 1) flipFrame = true;
			if (tail_whip) ani_walk_time = MARIO_ANI_TAIL_WHIP_TIME;
			else ani_walk_time = MARIO_ANI_SHOOT_TIME;
		}
	}

	if (tail_whip)
	{
		LPGAMEOBJECT object = NULL;
		if (PointCollision(*coObjects, pointTail_X, pointTail_Y, object))
		{
			if(object == NULL)
				DebugOut(L"NULL \n");
			else
			{
				if (object->GetType() == eType::ENEMY)
				{
					object->SetDirection(direction);
					object->SetState(ENEMY_STATE_HIT_TAIL);

					if (direction > 0)
						object->SetPosition(pointTail_X, pointTail_Y - object->GetHeight());
					else
						object->SetPosition(pointTail_X - object->GetWidth(), pointTail_Y - object->GetHeight());
				}
				else if (object->GetType() == eType::P_BLOCK)
				{
					if (object->GetState() == P_BLOCK_STATE_IDLE)
						object->SetState(P_BLOCK_STATE_HIT);
				}
				else
				{
					object->SetState(ACTIVE_BLOCK_STATE_HIT);
				}

			}
			
		}
	}

	//Flapping
	if (jumpButtonPressed)
	{
		SetState(MARIO_STATE_JUMP_FLAP_HOLD);
	}

	if (flapping == true)
	{
		if (isMax)
		{
			if (GetTickCount() - startFlapAni > MARIO_FLAP_RUN_DURATION)
				flapAni = false;
		}
		else
		{
			if (GetTickCount() - startFlapAni > MARIO_FLAP_DURATION)
				flapAni = false;
		}	
				
		if(flapAni)
			ani_walk_time = MARIO_ANI_FLAP_TIME;
			
		if (GetTickCount() - startFlap > MARIO_FLAP_TIME)
			flapping = false;
		else
			vy = MARIO_MAX_GRAVITY;
			
	}

	if (allowFlapJump)
	{
		if (GetTickCount() - startFlapJump > MARIO_FLAP_JUMP_TIME)
		{
			allowFlapJump = false;
		}
		
		if (GetTickCount() - flapDuration > MARIO_FLAP_JUMP_DURATION)
			flapJump = false;
		else
			vy = -MARIO_JUMP_SPEED_FLAP;



		global->speed = 7;

	}
	
	//Kicking
	if (kicking)
	{
		if (GetTickCount() - startKicking > MARIO_ANI_KICK_DURATION)
			kicking = false;
	}

	//Shoot
	if (allowShoot == false)
	{
		if (GetTickCount() - startShoot > MARIO_SHOOT_WAITING_TIME)
		{
			allowShoot = true;
		}
			
	}

	//Jump Crouch
	if (isCrouch && !touchGround)
		jumpCrouch = true;
	else if (touchGround)
		jumpCrouch = false;

	//Grabbing
	if (grabbing)
	{
		if (grabObject != NULL)
		{
			if (grabTurtlePress == false)
			{
				grabObject->SetDirection(direction);
				grabObject->SetState(ENEMY_STATE_KICK);
				grabObject = NULL;
				grabbing = false;
				directionGrab = 0;

				SetState(MARIO_STATE_KICK);
			}
			else
			{
				float x, y;
				int widthObject;
				grabObject->GetWidth(widthObject);

				if (switching == false)
				{
					if (direction > 0)
					{
						if (global->level != MARIO_LEVEL_FROG)
							x = this->x + width + MARIO_GRAB_OFFSET_X;
						else
							x = this->x + width + MARIO_GRAB_OFFSET_X - 5.0f;
					}
					else
						x = this->x - widthObject - MARIO_GRAB_OFFSET_X;

					if (global->level != 1100)
						y = this->y + MARIO_GRAB_OFFSET_Y;
					else
						y = this->y - 1.0f;

				}
				else //Switching
				{
					if (GetTickCount() - startSwitching >= MARIO_SWITCHING_DURATION * 2 / 3)
					{
						if (direction > 0)
						{
							if (global->level != MARIO_LEVEL_FROG)
								x = this->x + widthObject + MARIO_GRAB_OFFSET_X;
							else
								x = this->x + width + MARIO_GRAB_OFFSET_X - 5.0f;
						}
						else
							x = this->x - widthObject - MARIO_GRAB_OFFSET_X;
						switching = false;
					}
					else if (GetTickCount() - startSwitching >= MARIO_SWITCHING_DURATION * 1 / 3)
						x = this->x + (widthObject - width) / 2;
					else
					{
						if (direction < 0)
						{
							if (global->level != MARIO_LEVEL_FROG)
								x = this->x + widthObject + MARIO_GRAB_OFFSET_X;
							else
								x = this->x + width + MARIO_GRAB_OFFSET_X - 5.0f;
						}
						else
							x = this->x - widthObject - MARIO_GRAB_OFFSET_X;
					}

					if (global->level != 1100)
						y = this->y + MARIO_GRAB_OFFSET_Y;
					else
						y = this->y - 1.0f;

					ani_walk_time = MARIO_SWITCHING_TIME;
				}

				grabObject->SetPosition(x, y);
				grabObject->SetSpeed(vx, vy);
			}
		}
		else
			DebugOut(L"[ERROR] Nothing to grab!!!\n");
	}

	// Update camera to follow mario
	if (PAUSE == false)
		GameEngine::GetInstance()->UpdateCamPos(x, y);
}

void Mario::Render()
{
	//DebugOut(L"vx: %f - vy: %f\n", vx, vy);
	//DebugOut(L"State: %d\n", state);
	//DebugOut(L"Running: %d\n", isRunning);
	ani = -1;

	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else if (transporting)
	{
		if (state == MARIO_STATE_TRANSPORT_UP || state == MARIO_STATE_TRANSPORT_DOWN)
		{
			ani = global->level + MARIO_ANI_TRANSPORT;
		}
	}
	else if (inTransition == false)
	{
		if (tail_whip)
		{
			if (direction > 0) ani = global->level + MARIO_ANI_ATTACK_SET_RIGHT;
			else ani = global->level + MARIO_ANI_ATTACK_SET_LEFT;
		}
		else if (shoot)
		{
			if (direction > 0) ani = global->level + MARIO_ANI_ATTACK_SET_RIGHT;
			else ani = global->level + MARIO_ANI_ATTACK_SET_LEFT;
		}
		else if (kicking)
		{
			if (direction > 0) ani = global->level + MARIO_ANI_KICK_RIGHT;
			else ani = global->level + MARIO_ANI_KICK_LEFT;
		}
		else if (grabbing)
		{
			if (!touchGround)
			{
				if (direction > 0) ani = global->level + MARIO_ANI_HOLD_JUMP_RIGHT;
				else ani = global->level + MARIO_ANI_HOLD_JUMP_LEFT;
			}
			else if (switching)
			{
				if (direction > 0) ani = global->level + MARIO_ANI_HOLD_L_TO_R;
				else ani = global->level + MARIO_ANI_HOLD_R_TO_L;
			}
			else if (vx != 0)
			{
				if (direction > 0) ani = global->level + MARIO_ANI_HOLD_RIGHT;
				else ani = global->level + MARIO_ANI_HOLD_LEFT;
			}
			else
			{
				if (direction > 0) ani = global->level + MARIO_ANI_HOLD_IDLE_RIGHT;
				else ani = global->level + MARIO_ANI_HOLD_IDLE_LEFT;
			}
			
		}
		else if (!touchGround && !jumpCrouch)
		{
			if (startInvincible && global->level != MARIO_LEVEL_SMALL && global->level != MARIO_LEVEL_FROG)
			{
				if (direction > 0) ani = global->level + MARIO_ANI_ROLL_RIGHT;
				else ani = global->level + MARIO_ANI_ROLL_LEFT;
			}
			else if (flapAni)
			{
				if (direction > 0)
				{
					if (isMax || allowFlapJump) ani = global->level + MARIO_ANI_FLAP_RUN_RIGHT;
					else ani = global->level + MARIO_ANI_FLAP_RIGHT;
				}
				else
				{
					if (isMax || allowFlapJump) ani = global->level + MARIO_ANI_FLAP_RUN_LEFT;
					else ani = global->level + MARIO_ANI_FLAP_LEFT;
				}
			}
			else
			{
				if (direction > 0)
				{
					if (isMax || allowFlapJump) ani = global->level + MARIO_ANI_RUN_JUMP_RIGHT;
					else
					{
						if (global->level != MARIO_LEVEL_FROG)
						{
							if (vy < 0) ani = global->level + MARIO_ANI_JUMP_RIGHT;
							else ani = global->level + MARIO_ANI_JUMP_RIGHT_DOWN;
						}
						else
							ani = global->level + MARIO_ANI_JUMP_RIGHT;
					}
				}
				else
				{
					if (isMax || allowFlapJump) ani = global->level + MARIO_ANI_RUN_JUMP_LEFT;
					else
					{
						if (global->level != MARIO_LEVEL_FROG)
						{
							if (vy < 0) ani = global->level + MARIO_ANI_JUMP_LEFT;
							else ani = global->level + MARIO_ANI_JUMP_LEFT_DOWN;
						}
						else
							ani = global->level + MARIO_ANI_JUMP_LEFT;
					}
				}
	
			}

		}
		else if ((state == MARIO_STATE_CROUCH || jumpCrouch) && global->level != MARIO_LEVEL_SMALL && global->level != MARIO_LEVEL_FROG)
		{
			if (direction > 0) ani = global->level + MARIO_ANI_DUCK_RIGHT;
			else ani = global->level + MARIO_ANI_DUCK_LEFT;
		}
		else if (vx == 0)
		{
			if (direction > 0) ani = global->level + MARIO_ANI_IDLE_RIGHT;
			else ani = global->level + MARIO_ANI_IDLE_LEFT;
		}
		else if (state == MARIO_STATE_BREAK_LEFT && global->level != MARIO_LEVEL_FROG)
			ani = global->level + MARIO_ANI_BREAK_LEFT;
		else if (state == MARIO_STATE_BREAK_RIGHT && global->level != MARIO_LEVEL_FROG)
			ani = global->level + MARIO_ANI_BREAK_RIGHT;
		else if (vx > 0)
		{
			if (isMax) ani = global->level + MARIO_ANI_RUNNING_RIGHT;
			else ani = global->level + MARIO_ANI_WALKING_RIGHT;
		}
		else if (vx < 0)
		{
			if (isMax) ani = global->level + MARIO_ANI_RUNNING_LEFT;
			else ani = global->level + MARIO_ANI_WALKING_LEFT;
		}
	}

	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (inTransition)
	{
		if (state == MARIO_STATE_TRANSITION_1)
		{
			if (direction > 0) ani = BULLET_EFFECT_POP + BULLET_ANI_RIGHT;
			else ani = BULLET_EFFECT_POP + BULLET_ANI_LEFT;
			offset_Draw_X = (width - STANDARD_SIZE) / 2;
			offset_Draw_Y = (height - STANDARD_SIZE) / 2;
			ani_walk_time = 50;
		}
		else
		{
			ani_walk_time = 60;
			if (transitionUp)
			{
				if (direction > 0) ani =  MARIO_ANI_TRAN_RIGHT_UP;
				else ani = MARIO_ANI_TRAN_LEFT_UP;
			}
			else
			{
				if (direction > 0) ani = MARIO_ANI_TRAN_RIGHT_DOWN;
				else ani = MARIO_ANI_TRAN_LEFT_DOWN;
			}

		}
	}	
	else if (untouchable)
	{
		if (GetTickCount() - untouchable_frame > MARIO_ANI_UNTOUCHABLE_TIME)
		{
			color = D3DCOLOR_ARGB(128, 255, 255, 255);
			untouchable_frame = GetTickCount();
		}
		else
		{
			color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}
	else if (startInvincible)
	{
		color = InvicibilityPalette[indexPalette];

		if (GetTickCount() - invincible_frame > MARIO_ANI_INVINCIBLE_TIME)
		{
			++indexPalette;
			invincible_frame = GetTickCount();
		}
			

		if (indexPalette >= lengthPalette)
			indexPalette = 0;
	}
		

	animation_set->Get(ani)->SetTime(ani_walk_time);
	animation_set->Get(ani)->Render(x + offset_Draw_X, y + offset_Draw_Y, color);
	

	//this->sprite->Draw(x, y);

	//DebugOut(L"Direction: %d\n", direction);
	//RenderBoundingBox();
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();
	//DebugOut(L"State: %d\n", state);
	//DebugOut(L"----------\n");

	if (finished)
		return;

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (!touchRight)
		{
			if (global->level != MARIO_LEVEL_FROG)
			{
				tempVx = abs(vx);
				
				if (tempVx < MARIO_MAX_WALKING_SPEED)
					vx += MARIO_WALKING_SPEED * dt;
				else if (tempVx >= MARIO_MAX_WALKING_SPEED && tempVx < MARIO_MAX_WALKING_SPEED + 0.01f)
					vx = MARIO_MAX_WALKING_SPEED;
				else
					vx -= Global::Sign(vx) * MARIO_SLIDE_SPEED * dt;
			}
			direction = 1;
		}
		
		break;
	case MARIO_STATE_WALKING_RIGHT_FROG:
		if (!touchRight)
		{
			if (grabbing)
			{
				SetState(MARIO_STATE_RUNNING_RIGHT);
				break;
			}

			if (frog_jumping == false)
			{
				startFrogJump = now;
				frog_jumping = true;
			}	

			vx += MARIO_WALKING_SPEED * dt;

			if (abs(vx) > MARIO_MAX_WALKING_SPEED)
				vx = MARIO_MAX_WALKING_SPEED;

			direction = 1;
		}

		break;
	case MARIO_STATE_RUNNING_RIGHT:
		if (!touchGround && !isMax)
		{
			SetState(MARIO_STATE_WALKING_RIGHT);
		}
		else if (!touchRight)
		{
			vx += MARIO_RUNNING_SPEED * dt;

			if (abs(vx) > MARIO_MAX_RUNNING_SPEED_3)
			{
				vx = MARIO_MAX_RUNNING_SPEED_3;
				if (global->level != MARIO_LEVEL_FROG)
					isMax = true;
				else
					isMax = false;
			}
				
			direction = 1;
		}

		break;
	case MARIO_STATE_WALKING_LEFT:
		if (!touchLeft)
		{
			if (global->level != MARIO_LEVEL_FROG)
			{
				tempVx = abs(vx);

				if (tempVx < MARIO_MAX_WALKING_SPEED)
					vx += -MARIO_WALKING_SPEED * dt;
				else if (tempVx >= MARIO_MAX_WALKING_SPEED && tempVx < MARIO_MAX_WALKING_SPEED + 0.01f)
					vx = -MARIO_MAX_WALKING_SPEED;
				else
					vx -= Global::Sign(vx) * MARIO_SLIDE_SPEED * dt;

			}
			
			direction = -1;
		}
		
		break;
	case MARIO_STATE_WALKING_LEFT_FROG:
		if (!touchLeft)
		{
			if (grabbing)
			{
				SetState(MARIO_STATE_RUNNING_LEFT);
				break;
			}

			if (frog_jumping == false)
			{
				startFrogJump = now;
				frog_jumping = true;
			}
			vx += -MARIO_WALKING_SPEED * dt;

			if (abs(vx) > MARIO_MAX_WALKING_SPEED)
				vx = -MARIO_MAX_WALKING_SPEED;

			direction = -1;
		}

		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (!touchGround && !isMax)
		{
			SetState(MARIO_STATE_WALKING_LEFT);
		}
		else if (!touchLeft)
		{
			vx += -MARIO_RUNNING_SPEED * dt;

			if (abs(vx) > MARIO_MAX_RUNNING_SPEED_3)
			{
				vx = -MARIO_MAX_RUNNING_SPEED_3;
				if (global->level != MARIO_LEVEL_FROG)
					isMax = true;
				else
					isMax = false;
			}
			direction = -1;
		}

		break;
	case MARIO_STATE_BREAK_LEFT:
		if (isRunning) vx -= Global::Sign(vx) * MARIO_RUNNING_BREAK_SPEED * dt;
		else vx -= Global::Sign(vx) * MARIO_BREAK_SPEED * dt;
		if (vx < 0)	vx = 0;
		direction = -1;
		break;
	case MARIO_STATE_BREAK_RIGHT:
		if (isRunning) vx -= Global::Sign(vx) * MARIO_RUNNING_BREAK_SPEED * dt;
		else vx -= Global::Sign(vx) * MARIO_BREAK_SPEED * dt;
		if (vx > 0)	vx = 0;
		direction = 1;
		break;
	case MARIO_STATE_HOLD_SWITCH:
		if (direction != directionGrab)
		{
			directionGrab = direction;
			switching = true;
			startSwitching = now;
		}
		
		break;
	case MARIO_STATE_JUMP:
		jumpButtonPressed = true;

		if (global->level == MARIO_LEVEL_RACC || global->level == MARIO_LEVEL_TANU)
		{
			if (!allowFlapJump)
			{
				if (global->speed == 7)
				{
					allowFlapJump = true;
					flapJump = true;
					touchGround = false;

					startFlapJump = now;
					flapDuration = now;
					
					break;
				}
			}
		}

		if (startJumping == false)
		{
			jump_start = now;
			startJumping = true;
			jump_allow = true;
			touchGround = false;
		}

		//DebugOut(L"State: %d\n", state);
		break;
	case MARIO_STATE_STOP_JUMP:
		jumpButtonPressed = false;
		jump_allow = false;
		break;
	case MARIO_STATE_SHORT_JUMP:
		if (global->level == MARIO_LEVEL_RACC || global->level == MARIO_LEVEL_TANU)
		{
			if (!allowFlapJump)
			{
				if (global->speed == 7)
				{
					allowFlapJump = true;
					flapJump = true;
					startFlapJump = now;
					flapDuration = now;

				}
			}
		}	
		if (touchGround)
		{
			vy = -MARIO_JUMP_SPEED_Y;
			touchGround = false;

		}
		//DebugOut(L"dt: %d\n", dt);
		break;
	case MARIO_STATE_DEBUG_FLAP:
		if (global->level == MARIO_LEVEL_RACC || global->level == MARIO_LEVEL_TANU)
		{
			if (!allowFlapJump)
			{
				allowFlapJump = true;
				flapJump = true;
				startFlapJump = now;
				flapDuration = now;

			}
		}
		break;
	case MARIO_STATE_JUMP_FLAP_HOLD:
		jumpButtonPressed = true;

		if (allowFlapJump)
		{
			SetState(MARIO_STATE_JUMP_FLAP);
			break;
		}

		if (jump_allow == true)
			break;
		if (vy < 0)
			break;
		if (global->level == MARIO_LEVEL_RACC || global->level == MARIO_LEVEL_TANU)
		{
			startFlap = now;
			startFlapAni = now;
			flapping = true;
			flapAni = true;

		}
		break;
	case MARIO_STATE_JUMP_FLAP:
		if (global->level == MARIO_LEVEL_RACC || global->level == MARIO_LEVEL_TANU)
		{
			startFlap = now;
			startFlapAni = now;
			flapping = true;
			flapAni = true;
			
			if (allowFlapJump)
				flapDuration = now;			

		}
		break;
	case MARIO_STATE_CROUCH:
		if (global->level != MARIO_LEVEL_SMALL && global->level != MARIO_LEVEL_FROG && touchGround)
		{
			this->width = MARIO_BIG_CROUCH_BBOX_WIDTH;
			this->height = MARIO_BIG_CROUCH_BBOX_HEIGHT;
			if (isCrouch == false)
			{
				this->y = this->y + (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_CROUCH_BBOX_HEIGHT);
				isCrouch = true;
			}	
		}
		break;
	case MARIO_STATE_NOT_CROUCH:
		if (!jumpCrouch)
		{
			this->width = MARIO_BIG_BBOX_WIDTH;
			this->height = MARIO_BIG_BBOX_HEIGHT;
			this->y = this->y - (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_CROUCH_BBOX_HEIGHT);
			isCrouch = false;
		}
		break;
	case MARIO_STATE_INVINCIBLE:
		if (startInvincible == false)
		{
			invincible_start = now;
			invincible_frame = now;
			startInvincible = true;
			indexPalette = 0;
		}
		break;
	case MARIO_STATE_ATTACK:
		if (global->level == MARIO_LEVEL_FIRE || global->level == MARIO_LEVEL_HAM)
		{
			if (allowShoot == true)
			{
				shoot = true;
				allowShoot = false;
				startShoot = now;

				if (global->level == MARIO_LEVEL_FIRE)
					Fireball* bullet = new Fireball(pointX + direction * MARIO_BULLET_OFFSET_X, pointY + MARIO_BULLET_OFFSET_Y, direction);
				else
					Hammer* hammer = new Hammer(pointX + direction * MARIO_BULLET_OFFSET_X, pointY + MARIO_BULLET_OFFSET_Y, direction);
			}
			
		}
		else if (global->level == MARIO_LEVEL_RACC || global->level == MARIO_LEVEL_TANU)
		{
			if (tail_whip == false)
				tail_whip = true;
		}
		break;
	case MARIO_STATE_IDLE:
		if (vx != 0)
		{
			vx -= Global::Sign(vx) * MARIO_SLIDE_SPEED * dt;
			if (abs(vx) < MARIO_BREAK_THRESHOLD) vx = 0;
		}
		break;
	case MARIO_STATE_HOLD:
		grabTurtlePress = true;
		break;
	case MARIO_STATE_HOLD_SOMETHING:
		grabbing = true;
		directionGrab = direction;
		break;
	case MARIO_STATE_RELEASE:
		grabTurtlePress = false;
		break;
	case MARIO_STATE_RELEASE_FULL:
		if (grabbing)
		{
			grabbing = false;
			grabObject->SetState(ENEMY_STATE_RELEASE);
			grabTurtlePress = false;
			grabObject = NULL;
		}
		break;
	case MARIO_STATE_KICK:
		if (kicking == false)
		{
			kicking = true;
			startKicking = now;
		}
		break;
	case MARIO_STATE_HIT:
		if (startInvincible == false)
		{
			if (global->level != MARIO_LEVEL_SMALL)
			{
				untouchable_start = now;
				untouchable_frame = now;
				untouchable = true;
				type = eType::PLAYER_UNTOUCHABLE;
				PAUSE = true;

				if (global->level > MARIO_LEVEL_BIG)
				{
					global->level = MARIO_LEVEL_BIG;
					GameObject::SetState(MARIO_STATE_TRANSITION_1);
					startTransitionOne = now;
					SetAnimationSet(AnimationManager::GetInstance()->Get(BULLET));
				}
				else
				{
					oldLevel = MARIO_LEVEL_BIG;
					global->level = MARIO_LEVEL_SMALL;
					GameObject::SetState(MARIO_STATE_TRANSITION_2);
					startTransitionTwo = now;
					transitionUp = false;
				}

				inTransition = true;
				tail_whip = false;
				shoot = false;
				flapAni = false;
				flapping = false;
				allowFlapJump = false;
				jumpButtonPressed = false;
				
				if (grabbing)
				{
					grabbing = false;
					grabObject->SetState(ENEMY_STATE_RELEASE);
					grabTurtlePress = false;
					grabObject = NULL;
				}
			}
			else
			{
				if (grabbing)
				{
					grabbing = false;
					grabObject->SetState(ENEMY_STATE_RELEASE);
					grabTurtlePress = false;
					grabObject = NULL;
				}

				SetState(MARIO_STATE_DIE);
			}

		}
			
		break;
	case MARIO_STATE_UP:
		if (startInvincible == false)
		{
			untouchable_frame = now;
			type = eType::PLAYER_UNTOUCHABLE;
			PAUSE = true;

			if (oldLevel != MARIO_LEVEL_SMALL)
			{
				GameObject::SetState(MARIO_STATE_TRANSITION_1);
				startTransitionOne = now;
				SetAnimationSet(AnimationManager::GetInstance()->Get(BULLET));
			}
			else
			{
				GameObject::SetState(MARIO_STATE_TRANSITION_2);
				startTransitionTwo = now;
				transitionUp = true;
			}

			inTransition = true;
			tail_whip = false;
			shoot = false;
			flapAni = false;
			flapping = false;
			grabbing = false;
			grabTurtlePress = false;
			allowFlapJump = false;
			jumpButtonPressed = false;

		}

		break;
	case MARIO_STATE_TRANSPORT_UP:
		if (inTransition == false)
		{
			vx = 0;
			vy = -MARIO_TRANSPORT_SPEED;
			this->draw_order = ENEMY_ENTITY_PLANT_DRAW_ORDER;

			transporting = true;
			PAUSE = true;
			startTransport = now;
		}
		
		break;
	case MARIO_STATE_TRANSPORT_DOWN:
		if (inTransition == false)
		{
			vx = 0;
			vy = MARIO_TRANSPORT_SPEED;
			this->draw_order = ENEMY_ENTITY_PLANT_DRAW_ORDER;

			transporting = true;
			PAUSE = true;
			startTransport = now;
		}

		break;
	case MARIO_STATE_TRANSPORT_RESET:
		readyToSwitch = false;
		this->draw_order = PLAYER_DRAW_ORDER;
		break;
	case MARIO_STATE_FINISH:
		inTransition = false;
		tail_whip = false;
		shoot = false;
		flapAni = false;
		flapping = false;
		isMax = false;
		jumpButtonPressed = false;
		state = MARIO_STATE_IDLE;

		if (grabbing)
		{
			grabbing = false;
			grabObject->SetState(ENEMY_STATE_RELEASE);
			grabTurtlePress = false;
			grabObject = NULL;
		}

		global->finished = true;
		finished = true;
		startFinish = now;

		vx = 0;
		vy = 0;
		direction = 1;

		break;
	case MARIO_STATE_DIE:
		global->level = MARIO_LEVEL_SMALL;

		untouchable = false;
		startInvincible = false;
		inTransition = false;
		tail_whip = false;
		shoot = false;
		flapAni = false;
		flapping = false;
		allowFlapJump = false;
		jumpButtonPressed = false;

		if (grabbing)
		{
			grabbing = false;
			grabObject->SetState(ENEMY_STATE_RELEASE);
			grabTurtlePress = false;
			grabObject = NULL;
		}

		dying = true;
		die = true;
		die_time = now;

		global->die = true;
		global->allowCountTime = false;

		type = eType::ENEMY_MOB_DIE;

		PAUSE = true;

		break;
	}
}

void Mario::SetLevel(int l)
{
	this->global->level = l;

	if (prevLevel == l)
		return;

	switch (l)
	{
	case MARIO_LEVEL_SMALL:
		this->width = MARIO_SMALL_BBOX_WIDTH;
		this->height = MARIO_SMALL_BBOX_HEIGHT;
		this->offset_Draw_X = MARIO_OFFSET_SMALL_DRAW_X;
		this->offset_Draw_Y = MARIO_OFFSET_SMALL_DRAW_Y;
		this->offset_BBox_X = MARIO_OFFSET_SMALL_BBOX_X;
		this->offset_BBox_Y = MARIO_OFFSET_SMALL_BBOX_Y;

		this->y = this->y + (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);

		break;
		
	case MARIO_LEVEL_BIG:
	case MARIO_LEVEL_FIRE:
	case MARIO_LEVEL_RACC:
	case MARIO_LEVEL_TANU:
	case MARIO_LEVEL_HAM: 
	case LUIGI_LEVEL_BIG:
		this->width = MARIO_BIG_BBOX_WIDTH;
		this->height = MARIO_BIG_BBOX_HEIGHT;
		this->offset_Draw_X = MARIO_OFFSET_BIG_DRAW_X;
		this->offset_Draw_Y = MARIO_OFFSET_BIG_DRAW_Y;
		this->offset_BBox_X = MARIO_OFFSET_BIG_BBOX_X;
		this->offset_BBox_Y = MARIO_OFFSET_BIG_BBOX_Y;

		if (prevLevel == MARIO_LEVEL_FROG)
			this->y = this->y - (MARIO_BIG_BBOX_HEIGHT - MARIO_FROG_BBOX_HEIGHT);
		else if (prevLevel == MARIO_LEVEL_SMALL)
			this->y = this->y - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);

		break;

	case MARIO_LEVEL_FROG:
		this->width = MARIO_FROG_BBOX_WIDTH;
		this->height = MARIO_FROG_BBOX_HEIGHT;
		this->offset_Draw_X = MARIO_OFFSET_FROG_DRAW_X;
		this->offset_Draw_Y = MARIO_OFFSET_FROG_DRAW_Y;
		this->offset_BBox_X = MARIO_OFFSET_FROG_BBOX_X;
		this->offset_BBox_Y = MARIO_OFFSET_FROG_BBOX_Y;

		if (prevLevel == MARIO_LEVEL_SMALL)
			this->y = this->y - (MARIO_FROG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
		else
			this->y = this->y + (MARIO_BIG_BBOX_HEIGHT - MARIO_FROG_BBOX_HEIGHT);

		break;
	default:
		DebugOut(L"[ERROR] Unknown Mario global->level\n");
		break;
	}

	prevLevel = l;
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + offset_BBox_X;
	top = y + offset_BBox_Y;
	right = x + this->width;
	bottom = y + this->height;
}

bool Mario::PointCollision(vector<LPGAMEOBJECT>& coObjects, float pointX, float pointY)
{
	float left, top, right, bottom;

	for (LPGAMEOBJECT object : coObjects)
	{
		object->GetBoundingBox(left, top, right, bottom);

		if (right < this->x - STANDARD_SIZE)
			continue;
		if (left > this->x + this->width + STANDARD_SIZE)
			continue;

		switch (object->GetType())
		{
		case eType::GROUP:
		case eType::BLOCK:
		case eType::BRICK:
		case eType::QUESTION:
		case eType::P_BLOCK:
			//pointY += 16;

			//DebugOut(L"PointX: %f -- PointY: %f\n", pointX, pointY);
			//object->GetIndex(i, j);
			//DebugOut(L"i: %d -- j: %d\n", i, j);
			//DebugOut(L"Left: %f -- Top: %f -- Right: %f -- Bottom: %f\n", left, top, right, bottom);
			//DebugOut(L"Count: %d\n",count);

			if (pointX >= left && pointX <= right && pointY >= top && pointY <= bottom)
				return true;
		}

	}

	return false;
}

bool Mario::PointCollision(vector<LPGAMEOBJECT>& coObjects, float pointX, float pointY, LPGAMEOBJECT& target)
{
	float left, top, right, bottom;

	for (LPGAMEOBJECT object : coObjects)
	{
		if (object->GetType() != eType::ENEMY)
			if (object->GetType() != eType::QUESTION)
				if (object->GetType() != eType::BRICK)
					if (object->GetType() != eType::P_BLOCK)	
						continue;

		object->GetBoundingBox(left, top, right, bottom);

		if (right < this->x - STANDARD_SIZE)
			continue;
		if (left > this->x + this->width + STANDARD_SIZE)
			continue;

		switch (object->GetType())
		{
		case eType::ENEMY:
		case eType::QUESTION:
		case eType::BRICK:
		case eType::P_BLOCK:
			if (pointX >= left && pointX <= right && pointY >= top && pointY <= bottom)
			{
				target = object;
				return true;
			}
				
		}

	}

	return false;
}

bool Mario::isOutSideCam()
{
	if (y > global->screenHeight)
		return true;
	return false;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Mario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetSpeed(0, 0);	

	tail_whip = false;
	shoot = false;
	flapAni = false;
	flapping = false;

	PAUSE = false;
}

