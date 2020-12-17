#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include "Fireball.h"
#include "Hammer.h"
#include "Item.h"

//Speed
#define MARIO_MENU_WALKING_SPEED				0.0001f 
#define MARIO_MENU_RUNNING_SPEED				0.00008f 
#define MARIO_MENU_RUNNING_SPEED_FAST			0.0002f 
#define MARIO_MENU_RUNNING_SPEED_CUSTOM			0.0002f 
#define MARIO_MENU_JUMPING_SPEED				0.0025f
#define MARIO_MENU_MAX_WALKING_SPEED			0.1f
#define MARIO_MENU_HALF_MAX_RUNNING_SPEED		0.15f
#define MARIO_MENU_MAX_RUNNING_SPEED			0.2f
#define MARIO_MENU_MAX_FALLING_SPEED			0.3f
#define MARIO_MENU_SLIDE_SPEED					0.0002f
#define MARIO_MENU_BREAK_SPEED					0.0005f
#define MARIO_MENU_RUNNING_BREAK_SPEED			0.0002f

#define MARIO_MENU_SHORT_JUMP_SPEED_Y			0.2f
#define MARIO_MENU_JUMP_SPEED_Y					0.3f
#define MARIO_MENU_LONG_JUMP_SPEED_Y			0.32f
#define LUIGI_MENU_JUMP_SPEED_Y					0.45f
#define MARIO_MENU_GRAVITY						0.0007f
#define MARIO_MENU_MAX_GRAVITY					0.04f
#define MARIO_MENU_BREAK_THRESHOLD				0.005f

//State
#define MARIO_MENU_STATE_IDLE					0
#define MARIO_MENU_STATE_WALKING_RIGHT			100
#define MARIO_MENU_STATE_RUNNING_RIGHT			101
#define MARIO_MENU_STATE_WALK_RIGHT_NO_MOVE		103
#define MARIO_MENU_STATE_RUNNING_RIGHT_FAST		104
#define MARIO_MENU_STATE_RUNNING_RIGHT_CUS		105
#define MARIO_MENU_STATE_WALKING_LEFT			200
#define MARIO_MENU_STATE_RUNNING_LEFT			201
#define MARIO_MENU_STATE_WALK_LEFT_NO_MOVE		203
#define MARIO_MENU_STATE_RUNNING_LEFT_FAST		204
#define MARIO_MENU_STATE_RUNNING_LEFT_CUS		205
#define MARIO_MENU_STATE_BREAK_LEFT				300
#define MARIO_MENU_STATE_BREAK_RIGHT			400
#define MARIO_MENU_STATE_SHORT_JUMP				501
#define MARIO_MENU_STATE_JUMP_FLAP_HOLD			504
#define MARIO_MENU_STATE_STOP_JUMP				505
#define MARIO_MENU_STATE_LONG_JUMP				506
#define MARIO_MENU_STATE_VERY_SHORT_JUMP		507
#define MARIO_MENU_STATE_CROUCH					600
#define MARIO_MENU_STATE_NOT_CROUCH				601
#define MARIO_MENU_STATE_HOLD					801
#define MARIO_MENU_STATE_HOLD_SOMETHING			802
#define MARIO_MENU_STATE_RELEASE				803
#define MARIO_MENU_STATE_RELEASE_FULL			804
#define MARIO_MENU_STATE_KICK					805
#define MARIO_MENU_STATE_HIT					900
#define MARIO_MENU_STATE_UP						901
#define MARIO_MENU_STATE_TRANSITION_1			902
#define MARIO_MENU_STATE_TRANSITION_2			903
#define MARIO_MENU_STATE_HIT_BY_PLAYER			904
#define MARIO_MENU_STATE_HIT_BY_SHELL			905
#define MARIO_MENU_STATE_HIT_RELEASE			906
#define MARIO_MENU_STATE_TITLE_1				907
#define MARIO_MENU_STATE_TITLE_2				908
#define MARIO_MENU_STATE_HIT_RELEASE_2			909



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

	int ani_walk_time;
	float offset_Draw_X, offset_Draw_Y, offset_BBox_X, offset_BBox_Y;

	bool transitionUp = true;
	bool inTransition = false;
	DWORD startTransitionOne = 0;
	DWORD startTransitionTwo = 0;

	bool jump_allow = true;
	bool startJumping = false;
	DWORD jump_start = 0;

	bool flapAni = false;
	bool flapping = false;
	DWORD startFlap = 0;
	DWORD startFlapAni = 0;

	bool kicking = false;
	DWORD startKicking = 0;

	bool hitByLuigi = false;
	bool hitByShell = false;

	bool isRunning = false;
	bool isMax = false;
	bool isCrouch = false;
	bool grabTurtlePress = false;
	bool grabbing = false;
	bool jumpButtonPressed = false;

	int countTouch = 0;
	int countOffGround = 0;

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