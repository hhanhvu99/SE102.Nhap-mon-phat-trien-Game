#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include "Fireball.h"
#include "Hammer.h"
#include "Item.h"

//Speed
#define MARIO_WALKING_SPEED				0.00015f 
#define MARIO_RUNNING_SPEED				0.00008f 
#define MARIO_JUMPING_SPEED				0.0025f
#define MARIO_WALKING_FINISH_SPEED		0.05f
#define MARIO_MAX_WALKING_SPEED			0.1f
#define MARIO_HALF_MAX_RUNNING_SPEED_1	0.115f
#define MARIO_HALF_MAX_RUNNING_SPEED_2	0.135f
#define MARIO_HALF_MAX_RUNNING_SPEED_3	0.15f
#define MARIO_MAX_RUNNING_SPEED_1		0.165f
#define MARIO_MAX_RUNNING_SPEED_2		0.185f
#define MARIO_MAX_RUNNING_SPEED_3		0.2f
#define MARIO_MAX_FALLING_SPEED			0.2f
#define MARIO_SLIDE_SPEED				0.0002f
#define MARIO_BREAK_SPEED				0.0005f
#define MARIO_RUNNING_BREAK_SPEED		0.001f
#define MARIO_TRANSPORT_SPEED			0.03f

#define MARIO_BLOCK_DEFLECT				0.05f
#define MARIO_JUMP_SPEED_Y				0.3f
#define MARIO_JUMP_SPEED_FLAP			0.1f
#define MARIO_JUMP_DEFLECT_SPEED		0.15f
#define MARIO_GRAVITY					0.0007f
#define MARIO_MAX_GRAVITY				0.05f
#define MARIO_DIE_DEFLECT_SPEED			0.3f
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
#define MARIO_STATE_TRANSPORT_UP		401
#define MARIO_STATE_TRANSPORT_DOWN		402
#define MARIO_STATE_TRANSPORT_LEFT		403
#define MARIO_STATE_TRANSPORT_RIGHT		404
#define MARIO_STATE_TRANSPORT_RESET		405
#define MARIO_STATE_JUMP				500
#define MARIO_STATE_SHORT_JUMP			501
#define MARIO_STATE_CROUCH_JUMP			502
#define MARIO_STATE_JUMP_FLAP			503
#define MARIO_STATE_JUMP_FLAP_HOLD		504
#define MARIO_STATE_STOP_JUMP			505
#define MARIO_STATE_DEBUG_FLAP			506
#define MARIO_STATE_CROUCH				600
#define MARIO_STATE_NOT_CROUCH			601
#define MARIO_STATE_INVINCIBLE			700
#define MARIO_STATE_UNTOUCHABLE			701
#define MARIO_STATE_ATTACK				800
#define MARIO_STATE_HOLD				801
#define MARIO_STATE_HOLD_SOMETHING		802
#define MARIO_STATE_RELEASE				803
#define MARIO_STATE_RELEASE_FULL		804
#define MARIO_STATE_KICK				805
#define MARIO_STATE_HOLD_SWITCH			806
#define MARIO_STATE_HIT					900
#define MARIO_STATE_UP					901
#define MARIO_STATE_TRANSITION_1		902
#define MARIO_STATE_TRANSITION_2		903
#define MARIO_STATE_FINISH				999
#define MARIO_STATE_DIE					1000

//Time
#define MARIO_UNTOUCHABLE_TIME			2000
#define MARIO_TRANSITION_1_TIME			200
#define MARIO_TRANSITION_2_TIME			720
#define MARIO_INVINCIBILITY_TIME		5000	
#define MARIO_FROG_JUMPING_TIME			500
#define MARIO_SHOOT_WAITING_TIME		1000
#define MARIO_MAX_JUMPING				120
#define MARIO_FLAP_TIME					200
#define MARIO_FLAP_DURATION				100
#define MARIO_FLAP_RUN_DURATION			150
#define MARIO_FLAP_JUMP_TIME			5000
#define MARIO_FLAP_JUMP_DURATION		200
#define MARIO_ANI_KICK_DURATION			200
#define MARIO_SWITCHING_TIME			80
#define MARIO_SWITCHING_DURATION		240
#define MARIO_TRANSPORT_TIME			1000
#define MARIO_DIE_TIME					500
#define MARIO_DIE_TIME_SECOND			2000

#define MARIO_FINISH_TIME_1				2000
#define MARIO_FINISH_TIME_2				3000
#define MARIO_FINISH_TIME_3				4000
#define MARIO_FINISH_TIME_4				3000

//Point collide
#define MARIO_POINT_COLLIDE_OFFSET_X	8.0f
#define MARIO_POINT_COLLIDE_OFFSET_Y	0.5f
#define MARIO_POINT_TAIL_OFFSET_X		4.0f
#define MARIO_POINT_TAIL_OFFSET_Y		18.0f

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
//Grab
#define MARIO_GRAB_OFFSET_X				0.4f
#define MARIO_GRAB_OFFSET_Y				8.0f

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
protected:
	LPGAMEOBJECT grabObject;
	GLOBAL global;

	int oldLevel;
	int prevLevel;
	int ani;
	int size;
	int currentFrame;
	int directionGrab;

	float nx = 0, ny;

	float start_x;			// initial position of Mario at scene
	float start_y;
	float tempVx;
	float targetY;

	float pointX;
	float pointY;
	float pointX2;
	float pointY2;
	float pointTail_X;
	float pointTail_Y;

	int ani_walk_time;
	float offset_Draw_X, offset_Draw_Y, offset_BBox_X, offset_BBox_Y;

	bool flipFrame = false;

	bool untouchable = false;
	DWORD untouchable_start = 0;
	DWORD untouchable_frame = 0;

	bool transitionUp = true;
	bool inTransition = false;
	DWORD startTransitionOne = 0;
	DWORD startTransitionTwo = 0;
	DWORD transition_frame = 0;

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

	bool fallOutside = false;
	bool die = false;
	bool dying = false;
	DWORD die_time = 0;

	bool flapAni = false;
	bool flapping = false;
	DWORD startFlap = 0;
	DWORD startFlapAni = 0;

	bool allowFlapJump = false;
	bool flapJump = false;
	DWORD flapDuration = 0;
	DWORD startFlapJump = 0;

	bool kicking = false;
	DWORD startKicking = 0;

	bool switching = false;
	DWORD startSwitching = 0;

	bool transporting = false;
	bool readyToSwitch = false;
	DWORD startTransport = 0;

	bool finished = false;
	bool finishWaiting = false;
	DWORD startFinish = 0;

	bool firstText_1 = true;
	bool firstText_2 = true;
	bool firstIcon = true;

	bool touchLeft = false, touchRight = false;
	bool isRunning = false;
	bool isMax = false;
	bool isCrouch = false;
	bool jumpCrouch = false;
	bool grabTurtlePress = false;
	bool grabbing = false;
	bool jumpButtonPressed = false;

	int countTouch = 0;
	int countOffGround = 0;
	int lengthPalette;
	int indexPalette;

public:
	Mario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void Destroy() {};

	void SetGrabObject(LPGAMEOBJECT object) { grabObject = object; }
	void SetState(int state);
	void SetLevel(int l);
	void SetPositionBack(float min_tx, int nx) {x += min_tx * dx + nx * 3.0f;}
	void SetTargetY(float y) { targetY = y; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	int GetLevel() { return global->level; }

	bool PointCollision(vector<LPGAMEOBJECT>& collideObjects, float pointX, float pointY);
	bool PointCollision(vector<LPGAMEOBJECT>& collideObjects, float pointX, float pointY, LPGAMEOBJECT& target);

	bool isOutSideCam();
	bool isReady() { return readyToSwitch; }
	bool isCrouching() { return isCrouch; }
	bool isJumpFlap() { return flapJump; }
	bool isTouchGround() { return touchGround; }
	bool isAllowJump() { return jump_allow; }
	bool isGrappingPress() { return grabTurtlePress; }
	bool isGrapping() { return grabbing; }
	bool isDying() { return die; }
	bool isFinish() { return finished; }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};