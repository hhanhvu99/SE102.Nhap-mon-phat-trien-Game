#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include "Fireball.h"
#include "Hammer.h"

//Speed
#define MARIO_WALKING_SPEED				0.0002f 
#define MARIO_RUNNING_SPEED				0.0001f 
#define MARIO_JUMPING_SPEED				0.0035f
#define MARIO_MAX_WALKING_SPEED			0.1f
#define MARIO_HALF_MAX_RUNNING_SPEED	0.15f
#define MARIO_MAX_RUNNING_SPEED			0.2f
#define MARIO_SLIDE_SPEED				0.0002f
#define MARIO_BREAK_SPEED				0.0005f
#define MARIO_RUNNING_BREAK_SPEED		0.01f

#define MARIO_JUMP_SPEED_Y				0.5f
#define MARIO_JUMP_DEFLECT_SPEED		0.2f
#define MARIO_GRAVITY					0.0015f
#define MARIO_DIE_DEFLECT_SPEED			0.5f
#define MARIO_BREAK_THRESHOLD			0.005f

//State
#define MARIO_STATE_IDLE				0
#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_WALKING_RIGHT_FROG	101
#define MARIO_STATE_RUNNING_RIGHT		102
#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_WALKING_LEFT_FROG	201
#define MARIO_STATE_RUNNING_LEFT		202
#define MARIO_STATE_BREAK_LEFT			300
#define MARIO_STATE_BREAK_RIGHT			400
#define MARIO_STATE_JUMP				500
#define MARIO_STATE_SHORT_JUMP			501
#define MARIO_STATE_CROUCH_JUMP			502
#define MARIO_STATE_CROUCH				600
#define MARIO_STATE_NOT_CROUCH			601
#define MARIO_STATE_INVINCIBLE			700
#define MARIO_STATE_UNTOUCHABLE			701
#define MARIO_STATE_ATTACK				800
#define MARIO_STATE_DIE					1000

//Time
#define MARIO_UNTOUCHABLE_TIME			5000
#define MARIO_INVINCIBILITY_TIME		5000	
#define MARIO_FROG_JUMPING_TIME			500
#define MARIO_SHOOT_WAITING_TIME		1000
#define MARIO_MAX_JUMPING				120

//Point collide
#define MARIO_POINT_COLLIDE_OFFSET_X	4.0f
#define MARIO_POINT_COLLIDE_OFFSET_Y	0.5f

//Offset for different Mario level
//Small Mario
#define MARIO_OFFSET_SMALL_BBOX_X		0.0f
#define MARIO_OFFSET_SMALL_BBOX_Y		1.0f
#define MARIO_OFFSET_SMALL_DRAW_X		-1.0f
#define MARIO_OFFSET_SMALL_DRAW_Y		0.0f
//Big Mario
#define MARIO_OFFSET_BIG_BBOX_X			-1.0f
#define MARIO_OFFSET_BIG_BBOX_Y			1.0f
#define MARIO_OFFSET_BIG_DRAW_X			-1.0f
#define MARIO_OFFSET_BIG_DRAW_Y			-4.0f
#define MARIO_OFFSET_INVINCIBLE_DRAW_X	-2.0f
#define MARIO_OFFSET_INVINCIBLE_DRAW_Y	-2.0f
//Frog Mario
#define MARIO_OFFSET_FROG_BBOX_X		-1.0f
#define MARIO_OFFSET_FROG_BBOX_Y		1.0f
#define MARIO_OFFSET_FROG_DRAW_X		-1.0f
#define MARIO_OFFSET_FROG_DRAW_Y		-6.0f
//Bullet
#define MARIO_BULLET_OFFSET_X			-8.0f
#define MARIO_BULLET_OFFSET_Y			8.0f

//BBox
#define MARIO_SMALL_BBOX_WIDTH			13
#define MARIO_SMALL_BBOX_HEIGHT			15

#define MARIO_BIG_BBOX_WIDTH			14
#define MARIO_BIG_BBOX_HEIGHT			26
#define MARIO_BIG_ROLL_BBOX_WIDTH		20
#define MARIO_BIG_ROLL_BBOX_HEIGHT		26
#define MARIO_BIG_CROUCH_BBOX_WIDTH		14
#define MARIO_BIG_CROUCH_BBOX_HEIGHT	19

#define MARIO_FROG_BBOX_WIDTH			18
#define MARIO_FROG_BBOX_HEIGHT			24
			

class Mario : public GameObject
{
	int level;
	int ani;
	int size;
	int currentFrame;

	float nx = 0, ny;

	float start_x;			// initial position of Mario at scene
	float start_y;

	float pointX;
	float pointY;
	float pointX2;
	float pointY2;

	int ani_walk_time;
	float offset_Draw_X, offset_Draw_Y, offset_BBox_X, offset_BBox_Y;

	bool flipFrame = false;

	bool untouchable = false;
	DWORD untouchable_start;

	bool startInvincible = false;
	DWORD invincible_start = 0;
	DWORD invincible_frame = 0;

	bool jump_allow = true;
	bool startJumping = false;
	DWORD jump_start = 0;

	bool frog_jumping = false;
	DWORD startFrogJump = 0;

	bool tail_whip = false;
	DWORD startWhip = 0;

	bool allowShoot = true;
	bool shoot = false;
	DWORD startShoot = 0;

	bool touchLeft = false, touchRight = false;
	bool isRunning = false;
	bool isMax = false;
	bool isCrouch = false;
	bool jumpCrouch = false;

	int countTouch = 0;
	int countOffGround = 0;
	int lengthPalette;
	int indexPalette;

public:
	Mario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	int GetLevel() { return this->level; }

	bool PointCollision(vector<LPGAMEOBJECT>& collideObjects, float pointX, float pointY);
	bool isCrouching() { return isCrouch; }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};