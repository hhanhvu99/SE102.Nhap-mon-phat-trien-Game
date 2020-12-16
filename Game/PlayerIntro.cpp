#include <algorithm>
#include <assert.h>
#include "Debug.h"

#include "PlayerIntro.h"
#include "GameEngine.h"
#include "Global.h"


PlayerIntro::PlayerIntro(float x, float y) : GameObject()
{
	level = MARIO_LEVEL_BIG;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	this->type = eType::PLAYER;
	this->grabObject = NULL;
	this->directionGrab = 0;

	touchGround = false;
	ani_walk_time = MARIO_ANI_WALKING_TIME_DEFAULT;

	SetState(MARIO_MENU_STATE_IDLE);
	SetLevel(level);
	this->Reset();
}

void PlayerIntro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	// Simple fall down
	vy += MARIO_MENU_GRAVITY * dt;

	if (vy > MARIO_MENU_MAX_FALLING_SPEED)
		vy = MARIO_MENU_MAX_FALLING_SPEED;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents, { eType::ENEMY, eType::MARIO_BULLET, eType::ENEMY_MOB_DIE });

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
				SetLevel(level);
				type = eType::PLAYER;
			}
		}
		else
		{
			if (GetTickCount() - startTransitionTwo > MARIO_TRANSITION_2_TIME)
			{
				inTransition = false;
				PAUSE = false;
				SetLevel(level);
			}
		}

	}

	if (hitByLuigi)
	{
		SetState(MARIO_MENU_STATE_CROUCH);
	}

	// No collision occured, proceed normally
	//DebugOut(L"Size: %d \n", coEvents.size());
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

		// how to push back PlayerIntro if collides with a moving objects, what if PlayerIntro is pushed this way into another object?
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
					if (isCrouch) state = MARIO_MENU_STATE_CROUCH;
					else state = MARIO_MENU_STATE_IDLE;
					vx = 0;
				}
				else
				{
					if (isCrouch) state = MARIO_MENU_STATE_CROUCH;
					else state = MARIO_MENU_STATE_IDLE;
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
				case ITEM_SUPER_LEAF:
					oldLevel = level;
					level = MARIO_LEVEL_RACC;

					if (oldLevel == MARIO_LEVEL_BIG)
						SetState(MARIO_MENU_STATE_UP);
					else
						level = oldLevel;

					item->Destroy();
					break;
				default:
					break;
				}

				continue;
			}

			if (obj->GetType() == eType::PLAYER)
			{
				SetState(MARIO_MENU_STATE_HIT_BY_PLAYER);
				float vx, vy;
				obj->GetSpeed(vx, vy);
				obj->SetSpeed(vx, -LUIGI_MENU_JUMP_SPEED_Y);
			}

		}

	}

	//DebugOut(L"x: %f -- y:%f\n", x, y);
	//DebugOut(L"Is holding: %d\n", grabTurtlePress);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
		coEvents[i] = NULL;
	}

	//Is running or not?
	if (abs(vx) > MARIO_MENU_MAX_WALKING_SPEED)
		isRunning = true;
	else
		isRunning = false;

	//Update animation time
	float speed = abs(vx);
	if (level != MARIO_LEVEL_FROG)
	{
		if (speed < MARIO_MENU_MAX_WALKING_SPEED)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_DEFAULT;
			if (touchGround)
				isMax = false;
		}
		else if (speed == MARIO_MENU_MAX_WALKING_SPEED)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_WARMUP;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_MENU_HALF_MAX_RUNNING_SPEED)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_WARMUP;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_MENU_MAX_RUNNING_SPEED)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_RUN;
			if (touchGround)
				isMax = false;
		}
		else
			ani_walk_time = MARIO_ANI_WALKING_TIME_MAX;
	}
	else
	{
		ani_walk_time = MARIO_ANI_FROG_JUMPING_TIME;
	}
	//DebugOut(L"Ani time: %d \n", ani_walk_time);

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

				SetState(MARIO_MENU_STATE_KICK);
			}
			else
			{
				float x, y;
				int widthObject;
				grabObject->GetWidth(widthObject);

				if (direction > 0)
				{
					if (level != MARIO_LEVEL_FROG)
						x = this->x + widthObject + MARIO_GRAB_OFFSET_X;
					else
						x = this->x + width + MARIO_GRAB_OFFSET_X - 5.0f;
				}
				else
					x = this->x - widthObject - MARIO_GRAB_OFFSET_X;

				if (level != 1100)
					y = this->y + MARIO_GRAB_OFFSET_Y;
				else
					y = this->y - 1.0f;

				grabObject->SetPosition(x, y);
				grabObject->SetSpeed(0, 0);
			}
		}
		else
			DebugOut(L"[ERROR] Nothing to grab!!!\n");
	}

	//Jumping
	if (jump_allow)
	{
		if (GetTickCount() - jump_start < MARIO_MAX_JUMPING)
			vy -= (MARIO_MENU_JUMPING_SPEED + MARIO_MENU_GRAVITY) * dt;
		else
			jump_allow = false;
	}

	//Flapping
	if (jumpButtonPressed)
	{
		SetState(MARIO_MENU_STATE_JUMP_FLAP_HOLD);
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

		if (flapAni)
			ani_walk_time = MARIO_ANI_FLAP_TIME;

		if (GetTickCount() - startFlap > MARIO_FLAP_TIME)
			flapping = false;
		else
			vy = MARIO_MENU_MAX_GRAVITY;

	}

	//Kicking
	if (kicking)
	{
		if (GetTickCount() - startKicking > MARIO_ANI_KICK_DURATION)
			kicking = false;
	}

}

void PlayerIntro::Render()
{
	//DebugOut(L"State: %d\n", state);
	//DebugOut(L"Running: %d\n", isRunning);
	ani = -1;

	if (inTransition == false)
	{
		if (state == MARIO_MENU_STATE_TITLE_1)
		{
			ani = level + MARIO_ANI_TITLE_1;
		}
		else if (state == MARIO_MENU_STATE_TITLE_2)
		{
			ani = level + MARIO_ANI_TITLE_2;
		}
		else if (kicking)
		{
			if (direction > 0) ani = level + MARIO_ANI_KICK_RIGHT;
			else ani = level + MARIO_ANI_KICK_LEFT;
		}
		else if (grabbing)
		{
			if (!touchGround)
			{
				if (direction > 0) ani = level + MARIO_ANI_HOLD_JUMP_RIGHT;
				else ani = level + MARIO_ANI_HOLD_JUMP_LEFT;
			}
			else if (vx != 0)
			{
				if (direction > 0) ani = level + MARIO_ANI_HOLD_RIGHT;
				else ani = level + MARIO_ANI_HOLD_LEFT;
			}
			else
			{
				if (direction > 0) ani = level + MARIO_ANI_HOLD_IDLE_RIGHT;
				else ani = level + MARIO_ANI_HOLD_IDLE_LEFT;
			}

		}
		else if (!touchGround)
		{
			if (flapAni)
			{
				if (direction > 0)
				{
					if (isMax) ani = level + MARIO_ANI_FLAP_RUN_RIGHT;
					else ani = level + MARIO_ANI_FLAP_RIGHT;
				}
				else
				{
					if (isMax) ani = level + MARIO_ANI_FLAP_RUN_LEFT;
					else ani = level + MARIO_ANI_FLAP_LEFT;
				}
			}
			else
			{
				if (direction > 0)
				{
					if (isMax) ani = level + MARIO_ANI_RUN_JUMP_RIGHT;
					else
					{
						if (level != MARIO_LEVEL_FROG)
						{
							if (vy < 0) ani = level + MARIO_ANI_JUMP_RIGHT;
							else ani = level + MARIO_ANI_JUMP_RIGHT_DOWN;
						}
						else
							ani = level + MARIO_ANI_JUMP_RIGHT;
					}
				}
				else
				{
					if (isMax) ani = level + MARIO_ANI_RUN_JUMP_LEFT;
					else
					{
						if (level != MARIO_LEVEL_FROG)
						{
							if (vy < 0) ani = level + MARIO_ANI_JUMP_LEFT;
							else ani = level + MARIO_ANI_JUMP_LEFT_DOWN;
						}
						else
							ani = level + MARIO_ANI_JUMP_LEFT;
					}
				}

			}

		}
		else if (state == MARIO_MENU_STATE_CROUCH && level != MARIO_LEVEL_SMALL && level != MARIO_LEVEL_FROG)
		{
			if (direction > 0) ani = level + MARIO_ANI_DUCK_RIGHT;
			else ani = level + MARIO_ANI_DUCK_LEFT;
		}
		else if (vx == 0 && state != MARIO_MENU_STATE_WALK_LEFT_NO_MOVE && state != MARIO_MENU_STATE_WALK_RIGHT_NO_MOVE)
		{
			if (direction > 0) ani = level + MARIO_ANI_IDLE_RIGHT;
			else ani = level + MARIO_ANI_IDLE_LEFT;
		}
		else if (state == MARIO_MENU_STATE_BREAK_LEFT && level != MARIO_LEVEL_FROG)
			ani = level + MARIO_ANI_BREAK_LEFT;
		else if (state == MARIO_MENU_STATE_BREAK_RIGHT && level != MARIO_LEVEL_FROG)
			ani = level + MARIO_ANI_BREAK_RIGHT;
		else if (vx > 0 || state == MARIO_MENU_STATE_WALK_RIGHT_NO_MOVE)
		{
			if (isMax) ani = level + MARIO_ANI_RUNNING_RIGHT;
			else ani = level + MARIO_ANI_WALKING_RIGHT;
		}
		else if (vx < 0 || state == MARIO_MENU_STATE_WALK_LEFT_NO_MOVE)
		{
			if (isMax) ani = level + MARIO_ANI_RUNNING_LEFT;
			else ani = level + MARIO_ANI_WALKING_LEFT;
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
			if (GetTickCount() - transition_frame > MARIO_TRANSITION_2_CYCLE)
			{
				if (direction > 0) ani = oldLevel + MARIO_ANI_IDLE_RIGHT;
				else ani = oldLevel + MARIO_ANI_IDLE_LEFT;
				transition_frame = GetTickCount();
			}
			else
			{
				if (direction > 0) ani = MARIO_LEVEL_BIG + MARIO_ANI_TRANSITION_RIGHT;
				else ani = MARIO_LEVEL_BIG + MARIO_ANI_TRANSITION_LEFT;
				color = D3DCOLOR_ARGB(128, 255, 255, 255);
			}

		}
	}


	animation_set->Get(ani)->SetTime(ani_walk_time);
	animation_set->Get(ani)->Render(x + offset_Draw_X, y + offset_Draw_Y, color);


	//this->sprite->Draw(x, y);

	//DebugOut(L"Direction: %d\n", direction);
	RenderBoundingBox();
}

void PlayerIntro::SetState(int state)
{
	if (PAUSE == true)
		return;

	GameObject::SetState(state);
	DWORD now = GetTickCount();
	//DebugOut(L"State: %d\n", state);
	//DebugOut(L"----------\n");

	switch (state)
	{
	case MARIO_MENU_STATE_WALKING_RIGHT:
		if (grabbing)
		{
			SetState(MARIO_MENU_STATE_RUNNING_RIGHT);
			break;
		}

		if (level != MARIO_LEVEL_FROG)
		{
			tempVx = abs(vx);

			if (tempVx < MARIO_MENU_MAX_WALKING_SPEED)
				vx += MARIO_MENU_WALKING_SPEED * dt;
			else if (tempVx >= MARIO_MENU_MAX_WALKING_SPEED && tempVx < MARIO_MENU_MAX_WALKING_SPEED + 0.01f)
				vx = MARIO_MENU_MAX_WALKING_SPEED;
			else
				vx -= Global::Sign(vx) * MARIO_MENU_SLIDE_SPEED * dt;
		}
		direction = 1;
		

		break;
	case MARIO_MENU_STATE_WALK_RIGHT_NO_MOVE:
		direction = 1;
		break;
	case MARIO_MENU_STATE_RUNNING_RIGHT:
		vx += MARIO_MENU_RUNNING_SPEED * dt;

		if (abs(vx) > MARIO_MENU_MAX_RUNNING_SPEED)
		{
			vx = MARIO_MENU_MAX_RUNNING_SPEED;
			if (level != MARIO_LEVEL_FROG)
				isMax = true;
			else
				isMax = false;
		}

		direction = 1;
		break;
	case MARIO_MENU_STATE_RUNNING_RIGHT_FAST:
		vx += MARIO_MENU_RUNNING_SPEED_FAST * dt;

		if (abs(vx) > MARIO_MENU_MAX_RUNNING_SPEED)
		{
			vx = MARIO_MENU_MAX_RUNNING_SPEED;
			if (level != MARIO_LEVEL_FROG)
				isMax = true;
			else
				isMax = false;
		}

		direction = 1;

		break;
	case MARIO_MENU_STATE_RUNNING_RIGHT_CUS:
		vx += MARIO_MENU_RUNNING_SPEED_CUSTOM * dt;

		if (abs(vx) > MARIO_MENU_MAX_RUNNING_SPEED)
		{
			vx = MARIO_MENU_MAX_RUNNING_SPEED;
			if (level != MARIO_LEVEL_FROG)
				isMax = true;
			else
				isMax = false;
		}

		direction = 1;
		break;
	case MARIO_MENU_STATE_WALKING_LEFT:
		if (grabbing)
		{
			SetState(MARIO_MENU_STATE_RUNNING_LEFT);
			break;
		}

		if (level != MARIO_LEVEL_FROG)
		{
			tempVx = abs(vx);

			if (tempVx < MARIO_MENU_MAX_WALKING_SPEED)
				vx += -MARIO_MENU_WALKING_SPEED * dt;
			else if (tempVx >= MARIO_MENU_MAX_WALKING_SPEED && tempVx < MARIO_MENU_MAX_WALKING_SPEED + 0.01f)
				vx = -MARIO_MENU_MAX_WALKING_SPEED;
			else
				vx -= Global::Sign(vx) * MARIO_MENU_SLIDE_SPEED * dt;

		}

		direction = -1;

		break;
	case MARIO_MENU_STATE_WALK_LEFT_NO_MOVE:
		direction = -1;
		break;
	case MARIO_MENU_STATE_RUNNING_LEFT:
		vx += -MARIO_MENU_RUNNING_SPEED * dt;

		if (abs(vx) > MARIO_MENU_MAX_RUNNING_SPEED)
		{
			vx = -MARIO_MENU_MAX_RUNNING_SPEED;
			if (level != MARIO_LEVEL_FROG)
				isMax = true;
			else
				isMax = false;
		}
		direction = -1;
		break;
	case MARIO_MENU_STATE_RUNNING_LEFT_FAST:
		vx += -MARIO_MENU_RUNNING_SPEED_FAST * dt;

		if (abs(vx) > MARIO_MENU_MAX_RUNNING_SPEED)
		{
			vx = -MARIO_MENU_MAX_RUNNING_SPEED;
			if (level != MARIO_LEVEL_FROG)
				isMax = true;
			else
				isMax = false;
		}
		direction = -1;
		break;
	case MARIO_MENU_STATE_RUNNING_LEFT_CUS:
		vx += -MARIO_MENU_RUNNING_SPEED_CUSTOM * dt;

		if (abs(vx) > MARIO_MENU_MAX_RUNNING_SPEED)
		{
			vx = -MARIO_MENU_MAX_RUNNING_SPEED;
			if (level != MARIO_LEVEL_FROG)
				isMax = true;
			else
				isMax = false;
		}
		direction = -1;
		break;
	case MARIO_MENU_STATE_BREAK_LEFT:
		if (isRunning) vx -= Global::Sign(vx) * MARIO_MENU_RUNNING_BREAK_SPEED * dt;
		else vx -= Global::Sign(vx) * MARIO_MENU_BREAK_SPEED * dt;
		if (vx < 0)	vx = 0;
		direction = -1;
		break;
	case MARIO_MENU_STATE_BREAK_RIGHT:
		if (isRunning) vx -= Global::Sign(vx) * MARIO_MENU_RUNNING_BREAK_SPEED * dt;
		else vx -= Global::Sign(vx) * MARIO_MENU_BREAK_SPEED * dt;
		if (vx > 0)	vx = 0;
		direction = 1;
		break;
	case MARIO_MENU_STATE_STOP_JUMP:
		jumpButtonPressed = false;
		jump_allow = false;
		break;
	case MARIO_MENU_STATE_VERY_SHORT_JUMP:
		if (touchGround)
		{
			vy = -MARIO_MENU_SHORT_JUMP_SPEED_Y;
			touchGround = false;

		}
		//DebugOut(L"dt: %d\n", dt);
		break;
	case MARIO_MENU_STATE_SHORT_JUMP:
		if (touchGround)
		{
			vy = -MARIO_MENU_JUMP_SPEED_Y;
			touchGround = false;

		}
		//DebugOut(L"dt: %d\n", dt);
		break;
	case MARIO_MENU_STATE_LONG_JUMP:
		if (touchGround)
		{
			vy = -MARIO_MENU_LONG_JUMP_SPEED_Y;
			touchGround = false;

		}
		break;
	case MARIO_MENU_STATE_JUMP_FLAP_HOLD:
		jumpButtonPressed = true;
		if (jump_allow == true)
			break;
		if (vy < 0)
			break;
		if (level == MARIO_LEVEL_RACC || level == MARIO_LEVEL_TANU)
		{
			startFlap = now;
			startFlapAni = now;
			flapping = true;
			flapAni = true;

		}
		break;
	case MARIO_MENU_STATE_CROUCH:
		if (level != MARIO_LEVEL_SMALL && level != MARIO_LEVEL_FROG && touchGround)
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
	case MARIO_MENU_STATE_NOT_CROUCH:
		this->width = MARIO_BIG_BBOX_WIDTH;
		this->height = MARIO_BIG_BBOX_HEIGHT;
		this->y = this->y - (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_CROUCH_BBOX_HEIGHT);
		isCrouch = false;
		
		break;
	case MARIO_MENU_STATE_IDLE:
		if (vx != 0)
		{
			vx -= Global::Sign(vx) * MARIO_MENU_SLIDE_SPEED * dt;
			if (abs(vx) < MARIO_MENU_BREAK_THRESHOLD) vx = 0;
		}
		break;
	case MARIO_MENU_STATE_HOLD:
		grabTurtlePress = true;
		break;
	case MARIO_MENU_STATE_HOLD_SOMETHING:
		grabbing = true;
		directionGrab = direction;
		break;
	case MARIO_MENU_STATE_RELEASE:
		grabTurtlePress = false;
		break;
	case MARIO_MENU_STATE_RELEASE_FULL:
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
			SlowDown();
		}
		break;
	case MARIO_MENU_STATE_HIT_BY_PLAYER:
		vx = 0;
		hitByLuigi = true;
		break;
	case MARIO_MENU_STATE_HIT_RELEASE:
		hitByLuigi = false;
		SetState(MARIO_MENU_STATE_NOT_CROUCH);
		break;
	case MARIO_MENU_STATE_HIT_RELEASE_2:
		hitByShell = false;
		break;
	case MARIO_MENU_STATE_HIT_BY_SHELL:
		hitByShell = true;
		break;
	case MARIO_MENU_STATE_HIT:
		PAUSE = true;
		type = eType::PLAYER_UNTOUCHABLE;

		GameObject::SetState(MARIO_STATE_TRANSITION_2);
		startTransitionTwo = now;
		transition_frame = now;
		oldLevel = MARIO_LEVEL_BIG;
		level = MARIO_LEVEL_SMALL;


		inTransition = true;
		flapAni = false;
		flapping = false;

		if (grabbing)
		{
			grabbing = false;
			grabObject->SetState(ENEMY_STATE_RELEASE);
			grabTurtlePress = false;
			grabObject = NULL;
		}

		break;
	case MARIO_MENU_STATE_UP:
		type = eType::PLAYER_UNTOUCHABLE;
		PAUSE = true;

		GameObject::SetState(MARIO_STATE_TRANSITION_1);
		startTransitionOne = now;
		SetAnimationSet(AnimationManager::GetInstance()->Get(BULLET));

		inTransition = true;
		flapAni = false;
		flapping = false;
		grabbing = false;
		grabTurtlePress = false;

		break;
	
	default:
		break;
	}
}

void PlayerIntro::SetLevel(int l)
{
	this->level = l;

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
		DebugOut(L"[ERROR] Unknown PlayerIntro level\n");
		break;
	}

	prevLevel = l;
}

void PlayerIntro::SlowDown()
{
	if (vx > 0)
		vx -= 0.0025f * dt;
	else if (vx < 0)
		vx += 0.0025f * dt;
}

void PlayerIntro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + offset_BBox_X;
	top = y + offset_BBox_Y;
	right = x + this->width;
	bottom = y + this->height;
}

bool PlayerIntro::PointCollision(vector<LPGAMEOBJECT>& coObjects, float pointX, float pointY)
{
	float left, top, right, bottom;
	int i, j;

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

bool PlayerIntro::PointCollision(vector<LPGAMEOBJECT>& coObjects, float pointX, float pointY, LPGAMEOBJECT& target)
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
		case eType::ENEMY:
			if (pointX >= left && pointX <= right && pointY >= top && pointY <= bottom)
			{
				target = object;
				return true;
			}

		}

	}

	return false;
}

/*
	Reset PlayerIntro status to the beginning state of a scene
*/
void PlayerIntro::Reset()
{
	SetState(MARIO_MENU_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);

	flapAni = false;
	flapping = false;

	PAUSE = false;
}

