#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include "Fireball.h"
#include "Hammer.h"
#include "Item.h"

//Speed
#define MARIO_WALKING_SPEED				0.0001f 
#define MARIO_RUNNING_SPEED				0.00008f 
#define MARIO_RUNNING_SPEED_FAST		0.0002f 
#define MARIO_RUNNING_SPEED_CUSTOM		0.0002f 
#define MARIO_JUMPING_SPEED				0.0025f
#define MARIO_MAX_WALKING_SPEED			0.1f
#define MARIO_HALF_MAX_RUNNING_SPEED	0.15f
#define MARIO_MAX_RUNNING_SPEED			0.2f
#define MARIO_MAX_FALLING_SPEED			0.3f
#define MARIO_SLIDE_SPEED				0.0002f
#define MARIO_BREAK_SPEED				0.0005f
#define MARIO_RUNNING_BREAK_SPEED		0.0002f

#define MARIO_SHORT_JUMP_SPEED_Y		0.2f
#define MARIO_JUMP_SPEED_Y				0.3f
#define MARIO_LONG_JUMP_SPEED_Y			0.32f
#define LUIGI_JUMP_SPEED_Y				0.45f
#define MARIO_JUMP_DEFLECT_SPEED		0.15f
#define MARIO_GRAVITY					0.0007f
#define MARIO_MAX_GRAVITY				0.04f
#define MARIO_DIE_DEFLECT_SPEED			0.3f
#define MARIO_BREAK_THRESHOLD			0.005f

//State
#define MARIO_STATE_IDLE				0
#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_WALKING_RIGHT_FROG	101
#define MARIO_STATE_RUNNING_RIGHT		102
#define MARIO_STATE_WALK_RIGHT_NO_MOVE	103
#define MARIO_STATE_RUNNING_RIGHT_FAST	104
#define MARIO_STATE_RUNNING_RIGHT_CUS	105
#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_WALKING_LEFT_FROG	201
#define MARIO_STATE_RUNNING_LEFT		202
#define MARIO_STATE_WALK_LEFT_NO_MOVE	203
#define MARIO_STATE_RUNNING_LEFT_FAST	204
#define MARIO_STATE_RUNNING_LEFT_CUS	205
#define MARIO_STATE_BREAK_LEFT			300
#define MARIO_STATE_BREAK_RIGHT			400
#define MARIO_STATE_JUMP				500
#define MARIO_STATE_SHORT_JUMP			501
#define MARIO_STATE_CROUCH_JUMP			502
#define MARIO_STATE_JUMP_FLAP			503
#define MARIO_STATE_JUMP_FLAP_HOLD		504
#define MARIO_STATE_STOP_JUMP			505
#define MARIO_STATE_LONG_JUMP			506
#define MARIO_STATE_VERY_SHORT_JUMP		507
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
#define MARIO_STATE_HIT_BY_PLAYER		904
#define MARIO_STATE_HIT_BY_SHELL		905
#define MARIO_STATE_HIT_RELEASE			906
#define MARIO_STATE_TITLE_1				907
#define MARIO_STATE_TITLE_2				908
#define MARIO_STATE_HIT_RELEASE_2		909
#define MARIO_STATE_DIE					1000

//Time
#define MARIO_UNTOUCHABLE_TIME			2000
#define MARIO_TRANSITION_1_TIME			200
#define MARIO_TRANSITION_2_TIME			600
#define MARIO_TRANSITION_2_CYCLE		200
#define MARIO_INVINCIBILITY_TIME		5000	
#define MARIO_FROG_JUMPING_TIME			500
#define MARIO_SHOOT_WAITING_TIME		1000
#define MARIO_MAX_JUMPING				120
#define MARIO_FLAP_TIME					200
#define MARIO_FLAP_DURATION				100
#define MARIO_FLAP_RUN_DURATION			150
#define MARIO_ANI_KICK_DURATION			200
#define MARIO_SWITCHING_TIME			80
#define MARIO_SWITCHING_DURATION		240
#define MARIO_DIE_TIME					500

//Point collide
#define MARIO_POINT_COLLIDE_OFFSET_X	8.0f
#define MARIO_POINT_COLLIDE_OFFSET_Y	0.5f
#define MARIO_POINT_TAIL_OFFSET_X		4.0f
#define MARIO_POINT_TAIL_OFFSET_Y		18.0f

//Offset for different PlayerIntro level
//Small PlayerIntro
#define MARIO_OFFSET_SMALL_BBOX_X		0.0f
#define MARIO_OFFSET_SMALL_BBOX_Y		1.0f
#define MARIO_OFFSET_SMALL_DRAW_X		-1.0f
#define MARIO_OFFSET_SMALL_DRAW_Y		0.0f
//Big PlayerIntro
#define MARIO_OFFSET_BIG_BBOX_X			-1.0f
#define MARIO_OFFSET_BIG_BBOX_Y			1.0f
#define MARIO_OFFSET_BIG_DRAW_X			-1.0f
#define MARIO_OFFSET_BIG_DRAW_Y			-4.0f
#define MARIO_OFFSET_INVINCIBLE_DRAW_X	-2.0f
#define MARIO_OFFSET_INVINCIBLE_DRAW_Y	-2.0f
//Frog PlayerIntro
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


class PlayerIntro : public GameObject
{
protected:
	LPGAMEOBJECT grabObject;

	int level;
	int oldLevel;
	int prevLevel;
	int ani;
	int size;
	int currentFrame;
	int directionGrab;

	float nx = 0, ny;

	float start_x;			// initial position of PlayerIntro at scene
	float start_y;
	float tempVx;

	float anchor1_X;
	float anchor1_Y;
	float anchor2_X;
	float anchor2_Y;
	float anchor3_X;
	float anchor3_Y;

	float pointX;
	float pointY;
	float pointX2;
	float pointY2;
	float pointTail_X;
	float pointTail_Y;

	int ani_walk_time;
	float offset_Draw_X, offset_Draw_Y, offset_BBox_X, offset_BBox_Y;

	bool flipFrame = false;

	bool inTransition = false;
	DWORD startTransitionOne = 0;
	DWORD startTransitionTwo = 0;
	DWORD transition_frame = 0;


	bool jump_allow = true;
	bool startJumping = false;
	DWORD jump_start = 0;

	bool flapAni = false;
	bool flapping = false;
	DWORD startFlap = 0;
	DWORD startFlapAni = 0;

	bool kicking = false;
	DWORD startKicking = 0;

	bool switching = false;
	DWORD startSwitching = 0;

	bool hitByLuigi = false;
	bool hitByShell = false;

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
	PlayerIntro(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetGrabObject(LPGAMEOBJECT object) { grabObject = object; }
	void SetState(int state);
	void SetLevel(int l);
	void SetPositionBack(float min_tx, int nx) { x += min_tx * dx + nx * 3.0f; }
	void SlowDown();
	int GetLevel() { return this->level; }
	bool IsStop() { return vx == 0; }
	bool IsHitByShell() { return this->hitByShell; }

	bool PointCollision(vector<LPGAMEOBJECT>& collideObjects, float pointX, float pointY);
	bool PointCollision(vector<LPGAMEOBJECT>& collideObjects, float pointX, float pointY, LPGAMEOBJECT& target);

	bool isCrouching() { return isCrouch; }
	bool isTouchGround() { return touchGround; }
	bool isAllowJump() { return jump_allow; }
	bool isGrappingPress() { return grabTurtlePress; }
	bool isGrapping() { return grabbing; }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};