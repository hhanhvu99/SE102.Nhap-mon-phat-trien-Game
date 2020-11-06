#include <algorithm>
#include <assert.h>
#include "Debug.h"

#include "Mario.h"
#include "GameEngine.h"
#include "Global.h"


Mario::Mario(float x, float y) : GameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	this->type = eType::PLAYER;

	touchGround = false;
	ani_walk_time = MARIO_ANI_WALKING_TIME_DEFAULT;
	lengthPalette = sizeof(InvicibilityPalette) / sizeof(InvicibilityPalette[0]);
	indexPalette = 0;

	SetState(MARIO_STATE_IDLE);
	SetLevel(level);
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents, { eType::MARIO_BULLET, eType::ENEMY_MOB_DIE, eType::ENEMY });
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
				SetLevel(level);
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

	if (startInvincible)
	{
		if (GetTickCount() - invincible_start > MARIO_INVINCIBILITY_TIME)
		{
			invincible_start = 0;
			startInvincible = false;

			if (level != MARIO_LEVEL_SMALL && level != MARIO_LEVEL_FROG)
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
	}
	else if (direction > 0)
	{
		pointX = this->x + width + direction * (MARIO_POINT_COLLIDE_OFFSET_X);
		pointX2 = pointX;
	}
		
	pointY = this->y + MARIO_POINT_COLLIDE_OFFSET_Y;
	pointY2 = this->y + this->height - MARIO_POINT_COLLIDE_OFFSET_Y;

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
		//	x += nx*abs(rdx); 

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
				countOffGround = 0;
			}
			else if (ny > 0)
			{
				jump_allow = false;
			}

		}

		//float magnitude = sqrt(vx * vx + vy * vy);
		//int sign = Global::Sign(vy);

		
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

			if (ny > 0)
			{
				if (obj->GetType() == eType::BRICK)
				{
					obj->SetState(BRICK_SHINY_STATE_HIT);
				}
				else if (obj->GetType() == eType::QUESTION)
				{
					obj->SetState(QUESTION_BLOCK_STATE_HIT);
				}
			}
			/*
			else if (ny < 0 && touchGround == false)
			{
				if (obj->GetType() == eType::ENEMY)
				{
					obj->SetState(ENEMY_STATE_STOMP);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}*/
			
			
		}

	}

	//DebugOut(L"x: %f -- y:%f\n", x, y);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
		coEvents[i] = NULL;
	}

	//Is running or not?
	if (abs(vx) > MARIO_MAX_WALKING_SPEED)
		isRunning = true;
	else
		isRunning = false;

	//Update animation time
	float speed = abs(vx);
	if (level != MARIO_LEVEL_FROG)
	{
		if (speed < MARIO_MAX_WALKING_SPEED)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_DEFAULT;
			if (touchGround)
				isMax = false;
		}
		else if (speed == MARIO_MAX_WALKING_SPEED)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_WARMUP;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_HALF_MAX_RUNNING_SPEED)
		{
			ani_walk_time = MARIO_ANI_WALKING_TIME_WARMUP;
			if (touchGround)
				isMax = false;
		}
		else if (speed < MARIO_MAX_RUNNING_SPEED)
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

	//Invincibility BBox
	if (level != MARIO_LEVEL_SMALL && level != MARIO_LEVEL_FROG)
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

	//Attack
	if (ani != -1)
	{
		int attackLeft = level + MARIO_ANI_ATTACK_SET_LEFT;
		int attackRight = level + MARIO_ANI_ATTACK_SET_RIGHT;

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
		LPGAMEOBJECT enemy = NULL;
		if (PointCollision(*coObjects, pointX2, pointY2, enemy))
		{
			if(enemy == NULL)
				DebugOut(L"NULL \n");
			else
			{
				enemy->SetState(ENEMY_STATE_HIT);
				enemy->SetDirection(direction);
			}
			
		}
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

	// Update camera to follow mario
	float cx, cy;
	this->GetPosition(cx, cy);

	GameEngine* game = GameEngine::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	GameEngine::GetInstance()->SetCamPos(cx, 250.0f);
}

void Mario::Render()
{
	//DebugOut(L"State: %d\n", state);
	//DebugOut(L"Running: %d\n", isRunning);
	ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else if (inTransition == false)
	{
		if (tail_whip)
		{
			if (direction > 0) ani = level + MARIO_ANI_ATTACK_SET_RIGHT;
			else ani = level + MARIO_ANI_ATTACK_SET_LEFT;
		}
		else if (shoot)
		{
			if (direction > 0) ani = level + MARIO_ANI_ATTACK_SET_RIGHT;
			else ani = level + MARIO_ANI_ATTACK_SET_LEFT;
		}
		else if (!touchGround && !jumpCrouch)
		{
			if (startInvincible && level != MARIO_LEVEL_SMALL && level != MARIO_LEVEL_FROG)
			{
				if (direction > 0) ani = level + MARIO_ANI_ROLL_RIGHT;
				else ani = level + MARIO_ANI_ROLL_LEFT;
			}
			else
			{
				if (direction > 0)
				{
					if (isMax) ani = level + MARIO_ANI_RUN_JUMP_RIGHT;
					else ani = level + MARIO_ANI_JUMP_RIGHT;
				}
				else
				{
					if (isMax) ani = level + MARIO_ANI_RUN_JUMP_LEFT;
					else ani = level + MARIO_ANI_JUMP_LEFT;
				}
			}

		}
		else if ((state == MARIO_STATE_CROUCH || jumpCrouch) && level != MARIO_LEVEL_SMALL && level != MARIO_LEVEL_FROG)
		{
			if (direction > 0) ani = level + MARIO_ANI_DUCK_RIGHT;
			else ani = level + MARIO_ANI_DUCK_LEFT;
		}
		else if (vx == 0)
		{
			if (direction > 0) ani = level + MARIO_ANI_IDLE_RIGHT;
			else ani = level + MARIO_ANI_IDLE_LEFT;
		}
		else if (state == MARIO_STATE_BREAK_LEFT && level != MARIO_LEVEL_FROG)
			ani = level + MARIO_ANI_BREAK_LEFT;
		else if (state == MARIO_STATE_BREAK_RIGHT && level != MARIO_LEVEL_FROG)
			ani = level + MARIO_ANI_BREAK_RIGHT;
		else if (vx > 0)
		{
			if (isMax) ani = level + MARIO_ANI_RUNNING_RIGHT;
			else ani = level + MARIO_ANI_WALKING_RIGHT;
		}
		else if (vx < 0)
		{
			if (isMax) ani = level + MARIO_ANI_RUNNING_LEFT;
			else ani = level + MARIO_ANI_WALKING_LEFT;
		}
	}

	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (untouchable)
	{
		if (inTransition == false)
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
		else
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
					if (direction > 0) ani = MARIO_LEVEL_BIG + MARIO_ANI_IDLE_RIGHT;
					else ani = MARIO_LEVEL_BIG + MARIO_ANI_IDLE_LEFT;
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

	//DebugOut(L"Ani id: %d\n", ani);
	/*
	if (ani == MARIO_ANI_FIRE_SHOOT_RIGHT)
	{
		DebugOut(L"Fire!!!\n");
		DebugOut(L"Ani id: %d\n", ani);
	}*/
		
	
	animation_set->Get(ani)->SetTime(ani_walk_time);
	animation_set->Get(ani)->Render(x + offset_Draw_X, y + offset_Draw_Y, color);

	//this->sprite->Draw(x, y);

	//DebugOut(L"Direction: %d\n", direction);
	RenderBoundingBox();
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);
	DWORD now = GetTickCount();
	//DebugOut(L"State: %d\n", state);
	//DebugOut(L"----------\n");

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (!touchRight)
		{
			if (level != MARIO_LEVEL_FROG)
			{
				if (abs(vx) + MARIO_WALKING_SPEED <= MARIO_MAX_WALKING_SPEED)
					vx += MARIO_WALKING_SPEED * dt;
				else
					vx = MARIO_MAX_WALKING_SPEED;
			}
			direction = 1;
		}
		
		break;
	case MARIO_STATE_WALKING_RIGHT_FROG:
		if (!touchRight)
		{
			if (frog_jumping == false)
			{
				startFrogJump = now;
				frog_jumping = true;
			}	
			if (abs(vx) + MARIO_WALKING_SPEED <= MARIO_MAX_WALKING_SPEED)
				vx += MARIO_WALKING_SPEED * dt;
			else
				vx = MARIO_MAX_WALKING_SPEED;

			direction = 1;
		}

		break;
	case MARIO_STATE_RUNNING_RIGHT:
		if (!touchRight)
		{
			if (abs(vx) + MARIO_RUNNING_SPEED <= MARIO_MAX_RUNNING_SPEED)
				vx += MARIO_RUNNING_SPEED * dt;
			else
			{
				vx = MARIO_MAX_RUNNING_SPEED;
				isMax = true;
			}
			direction = 1;
		}

		break;
	case MARIO_STATE_WALKING_LEFT:
		if (!touchLeft)
		{
			if (level != MARIO_LEVEL_FROG)
			{
				if (abs(vx) + MARIO_WALKING_SPEED <= MARIO_MAX_WALKING_SPEED)
					vx += -MARIO_WALKING_SPEED * dt;
				else
					vx = -MARIO_MAX_WALKING_SPEED;
			}
			
			direction = -1;
		}
		
		break;
	case MARIO_STATE_WALKING_LEFT_FROG:
		if (!touchLeft)
		{
			if (frog_jumping == false)
			{
				startFrogJump = now;
				frog_jumping = true;
			}
			if (abs(vx) + MARIO_WALKING_SPEED <= MARIO_MAX_WALKING_SPEED)
				vx += -MARIO_WALKING_SPEED * dt;
			else
				vx = -MARIO_MAX_WALKING_SPEED;

			direction = -1;
		}

		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (!touchLeft)
		{
			if (abs(vx) + MARIO_RUNNING_SPEED <= MARIO_MAX_RUNNING_SPEED)
				vx += -MARIO_RUNNING_SPEED * dt;
			else
			{
				vx = -MARIO_MAX_RUNNING_SPEED;
				isMax = true;
			}
			direction = -1;
		}

		break;
	case MARIO_STATE_BREAK_LEFT:
		if (isRunning) vx -= Global::Sign(vx) * MARIO_RUNNING_BREAK_SPEED;
		else vx -= Global::Sign(vx) * MARIO_BREAK_SPEED * dt;
		if (vx < 0) vx = 0;
		direction = -1;
		break;
	case MARIO_STATE_BREAK_RIGHT:
		if (isRunning) vx -= Global::Sign(vx) * MARIO_RUNNING_BREAK_SPEED;
		else vx -= Global::Sign(vx) * MARIO_BREAK_SPEED * dt;
		if (vx > 0) vx = 0;
		direction = 1;
		break;
	case MARIO_STATE_JUMP:
		if (startJumping == false)
		{
			jump_start = now;
			startJumping = true;
			touchGround = false;
		}

		if (now - jump_start < MARIO_MAX_JUMPING && jump_allow)
		{
			vy -= (MARIO_JUMPING_SPEED + MARIO_GRAVITY) * dt;

			if (now - jump_start >= MARIO_MAX_JUMPING)
				jump_allow = false;
		}

		if (touchGround)
		{
			startJumping = false;
			jump_allow = true;
		}
		//DebugOut(L"State: %d\n", state);
		break;
	case MARIO_STATE_SHORT_JUMP:
		if (touchGround)
		{
			vy = -MARIO_JUMP_SPEED_Y;
			touchGround = false;

		}
		//DebugOut(L"dt: %d\n", dt);
		break;
	case MARIO_STATE_CROUCH:
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
		if (level == MARIO_LEVEL_FIRE || level == MARIO_LEVEL_HAM)
		{
			if (allowShoot == true)
			{
				shoot = true;
				allowShoot = false;
				startShoot = now;

				if (level == MARIO_LEVEL_FIRE)
					Fireball* bullet = new Fireball(pointX + direction * MARIO_BULLET_OFFSET_X, pointY + MARIO_BULLET_OFFSET_Y, direction);
				else
					Hammer* hammer = new Hammer(pointX + direction * MARIO_BULLET_OFFSET_X, pointY + MARIO_BULLET_OFFSET_Y, direction);
			}
			
		}
		else if (level == MARIO_LEVEL_RACC || level == MARIO_LEVEL_TANU)
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
	case MARIO_STATE_HIT:
		if (level != MARIO_LEVEL_SMALL)
		{
			untouchable_start = now;
			untouchable_frame = now;
			untouchable = true;
			type = eType::PLAYER_UNTOUCHABLE;
			PAUSE = true;

			if (level > MARIO_LEVEL_BIG)
			{
				level = MARIO_LEVEL_BIG;
				GameObject::SetState(MARIO_STATE_TRANSITION_1);
				startTransitionOne = now;
				SetAnimationSet(AnimationManager::GetInstance()->Get(BULLET));
			}
			else
			{
				level = MARIO_LEVEL_SMALL;
				GameObject::SetState(MARIO_STATE_TRANSITION_2);
				startTransitionTwo = now;
				transition_frame = now;
			}

			inTransition = true;
			tail_whip = false;
			shoot = false;
		}
		else
		{
			SetState(MARIO_STATE_DIE);
		}
		
			
		break;
	case MARIO_STATE_DIE:
		PAUSE = true;
		dying = true;
		die_time = now;

		break;
	}
}

void Mario::SetLevel(int l)
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

		this->x = this->x;
		this->y = this->y + (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);

		break;
		
	case MARIO_LEVEL_BIG:
	case MARIO_LEVEL_FIRE:
	case MARIO_LEVEL_RACC:
	case MARIO_LEVEL_TANU:
	case MARIO_LEVEL_HAM: 
		this->width = MARIO_BIG_BBOX_WIDTH;
		this->height = MARIO_BIG_BBOX_HEIGHT;
		this->offset_Draw_X = MARIO_OFFSET_BIG_DRAW_X;
		this->offset_Draw_Y = MARIO_OFFSET_BIG_DRAW_Y;
		this->offset_BBox_X = MARIO_OFFSET_BIG_BBOX_X;
		this->offset_BBox_Y = MARIO_OFFSET_BIG_BBOX_Y;
		break;

	case MARIO_LEVEL_FROG:
		this->width = MARIO_FROG_BBOX_WIDTH;
		this->height = MARIO_FROG_BBOX_HEIGHT;
		this->offset_Draw_X = MARIO_OFFSET_FROG_DRAW_X;
		this->offset_Draw_Y = MARIO_OFFSET_FROG_DRAW_Y;
		this->offset_BBox_X = MARIO_OFFSET_FROG_BBOX_X;
		this->offset_BBox_Y = MARIO_OFFSET_FROG_BBOX_Y;

		break;
	default:
		DebugOut(L"[ERROR] Unknown Mario level\n");
		break;
	}

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

bool Mario::PointCollision(vector<LPGAMEOBJECT>& coObjects, float pointX, float pointY, LPGAMEOBJECT& target)
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
	Reset Mario status to the beginning state of a scene
*/
void Mario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(level);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	PAUSE = false;
}

