#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240 
#define MAX_FRAME_RATE 120
#define STANDARD_SIZE 16

//Define Draw Order
#define BACKGROUND_DRAW_ORDER	0
#define ENEMY_ENTITY_DRAW_ORDER	1
#define BLOCK_DRAW_ORDER		2
#define ACTIVE_BLOCK_DRAW_ORDER	3
#define PLAYER_DRAW_ORDER		4
#define BULLET_DRAW_ORDER		5


/*Define Active Block*/
//Animation
#define ACTIVE_BLOCK 1134
#define BLOCK_ANI_TIME 150

#define BRICK_SHINY_ANI			91
#define QUESTION_BLOCK_ANI		251
#define QUESTION_BLOCK_ANI_HIT	255

//State
//Object
#define BRICK_SHINY_STATE_DESTROY		0
#define BRICK_SHINY_STATE_NORMAL		1
#define BRICK_SHINY_STATE_MOVING		2
#define BRICK_SHINY_STATE_MULTIPLE		3
#define BRICK_SHINY_STATE_HIT			4

#define QUESTION_BLOCK_STATE_NORMAL		1
#define QUESTION_BLOCK_STATE_MOVING		2
#define QUESTION_BLOCK_STATE_HIT		3

#define MOVING_SPEED					0.3f
#define DEFLECT_SPEED					0.05f
#define BOUNDARY						0.8f

#define BLOCK_MOVING_TIME				20

//Entity
#define ENTITY_SAFE_DELETE_RANGE		300.0f

//Animation
#define MARIO							1412

//Offset
#define MARIO_ANI_BIG_RUN_JUMP_OFFSET_X			-7.0f
#define MARIO_ANI_BIG_WALK_JUMP_OFFSET_X		-1.0f
#define MARIO_ANI_FIRE_RUN_JUMP_OFFSET_X		-7.0f
#define MARIO_ANI_FIRE_WALK_JUMP_OFFSET_X		-1.0f
#define MARIO_ANI_HAM_RUN_JUMP_OFFSET_X			-7.0f
#define MARIO_ANI_TANU_RACC_WALK_OFFSET_X		-8.0f
#define MARIO_ANI_TANU_RACC_RUN_OFFSET_X		-7.0f
#define MARIO_FROG_JUMP_OFFSET_X				-5.0f

//Invicibility palette

static const D3DCOLOR InvicibilityPalette[] = {
	D3DCOLOR_ARGB(255, 255, 255, 255),
	D3DCOLOR_ARGB(255, 232, 157, 52),
	D3DCOLOR_ARGB(255, 152, 78, 17),
	D3DCOLOR_ARGB(255, 99, 99, 99),
	D3DCOLOR_ARGB(255, 178, 50, 38),
	D3DCOLOR_ARGB(255, 5, 64, 76),
	D3DCOLOR_ARGB(255, 98, 226, 64),
	D3DCOLOR_ARGB(255, 79, 205, 221),
	D3DCOLOR_ARGB(255, 173, 173, 173),
	D3DCOLOR_ARGB(255, 104, 178, 252),
};

//Pause Movement
extern bool PAUSE;

//Animation Time
#define MARIO_ANI_WALKING_TIME_DEFAULT	150
#define MARIO_ANI_WALKING_TIME_WARMUP	80
#define MARIO_ANI_WALKING_TIME_RUN		60
#define MARIO_ANI_WALKING_TIME_MAX		30

#define MARIO_ANI_INVINCIBLE_TIME		10
#define MARIO_ANI_UNTOUCHABLE_TIME		50

#define MARIO_ANI_FROG_JUMPING_TIME		160
#define MARIO_ANI_TAIL_WHIP_TIME		60
#define MARIO_ANI_SHOOT_TIME			40
#define MARIO_ANI_FLAP_TIME				50

////////////////
//Mario Prefix//
////////////////
//Level Mario
#define	MARIO_LEVEL_SMALL				1100
#define	MARIO_LEVEL_BIG					1200
#define	MARIO_LEVEL_FIRE				1300
#define	MARIO_LEVEL_RACC				1400
#define	MARIO_LEVEL_FROG				1500
#define	MARIO_LEVEL_TANU				1600
#define	MARIO_LEVEL_HAM					1700

////////////////
//Mario Suffix//
////////////////
//Idle
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_IDLE_RIGHT		2
//Walking
#define MARIO_ANI_WALKING_LEFT		3
#define MARIO_ANI_WALKING_RIGHT		6
//Running
#define MARIO_ANI_RUNNING_LEFT		9
#define MARIO_ANI_RUNNING_RIGHT		12
//Jumping
#define MARIO_ANI_JUMP_LEFT			15
#define MARIO_ANI_RUN_JUMP_LEFT		16
#define MARIO_ANI_JUMP_RIGHT		17
#define MARIO_ANI_RUN_JUMP_RIGHT	18
//Break
#define MARIO_ANI_BREAK_LEFT		19
#define MARIO_ANI_BREAK_RIGHT 		20
//Duck
#define MARIO_ANI_DUCK_LEFT			21
#define MARIO_ANI_DUCK_RIGHT		22
//Transport
#define MARIO_ANI_TRANSPORT			23
//Hold
#define MARIO_ANI_HOLD_LEFT			24
#define MARIO_ANI_HOLD_RIGHT		27
//Kick
#define MARIO_ANI_KICK_LEFT			30
#define MARIO_ANI_KICK_RIGHT		31
//Swim
#define MARIO_ANI_SWIM_LEFT			32
#define MARIO_ANI_SWIM_RIGHT		36
#define MARIO_ANI_SWIM_UP			40
//Barrel Roll
#define MARIO_ANI_ROLL_LEFT			42
#define MARIO_ANI_ROLL_RIGHT		50
//Slide
#define MARIO_ANI_SLIDE_LEFT		58
#define MARIO_ANI_SLIDE_RIGHT		59
//Attack
#define MARIO_ANI_ATTACK_LEFT		68
#define MARIO_ANI_ATTACK_RIGHT		70
//Flap Tail	
#define MARIO_ANI_FLAP_LEFT			72
#define MARIO_ANI_FLAP_RUN_LEFT		73
#define MARIO_ANI_FLAP_RIGHT		74
#define MARIO_ANI_FLAP_RUN_RIGHT	75
//Attack Set
#define MARIO_ANI_ATTACK_SET_LEFT	76
#define MARIO_ANI_ATTACK_SET_RIGHT	77
//Statues
#define MARIO_ANI_STAUTES			78
//Transition
#define MARIO_ANI_TRANSITION_LEFT	79
#define MARIO_ANI_TRANSITION_RIGHT	80
//Title Screen
#define MARIO_ANI_TITLE_1			81
#define MARIO_ANI_TITLE_2			82
//Jump Extra
#define MARIO_ANI_JUMP_LEFT_DOWN	83
#define MARIO_ANI_JUMP_RIGHT_DOWN	84
//Hold Extra
#define MARIO_ANI_HOLD_IDLE_LEFT	85
#define MARIO_ANI_HOLD_IDLE_RIGHT	86
#define MARIO_ANI_HOLD_JUMP_LEFT	87
#define MARIO_ANI_HOLD_JUMP_RIGHT	88
//Die
#define MARIO_ANI_DIE				1800

//////////
//Bullet//
//////////
#define BULLET							3577
 
//Bullet State
#define BULLET_STATE_IDLE				0
#define BULLET_STATE_MOVING				1
#define BULLET_STATE_HIT				2	

/////////////////
//Bullet Prefix//
/////////////////
//Bullet Type
#define BULLET_FIREBALL_TYPE_NORMAL		10000
#define BULLET_FIREBALL_TYPE_DUNGEON	20000
#define BULLET_FIREBALL_TYPE_TOAD		30000
#define BULLET_HAMMER_TYPE				40000
#define BULLET_EFFECT_POP				50000

/////////////////
//Bullet Suffix//
/////////////////
#define BULLET_ANI_LEFT					9
#define BULLET_ANI_RIGHT				10

//Time
#define BULLET_FIREBALL_EFFECT_SPEED	20
#define BULLET_FIREBALL_ANI_SPEED		50
#define BULLET_HAMMER_ANI_SPEED			70

/////////////
//Enemy Mob//
/////////////
#define ENEMY_MOB						7791

//////////////
//Mob Prefix//
//////////////
//Mob Type
#define ENEMY_GROOMBA_BROWN				100000
#define ENEMY_GROOMBA_RED				200000
#define ENEMY_KOOPAS_GREEN				300000
#define ENEMY_KOOPAS_RED				400000
#define ENEMY_PIRANHA_GREEN				500000
#define ENEMY_VENUS_GREEN				600000
#define ENEMY_VENUS_RED					700000

//////////////
//Mob Suffix//
//////////////
#define ENEMY_ANI_LEFT					9
#define ENEMY_ANI_RIGHT					10
#define ENEMY_ANI_IMMOBILIZE			11
#define ENEMY_ANI_ROLLING				12
#define ENEMY_ANI_COMEBACK				13
#define ENEMY_ANI_LEFT_DOWN				14
#define ENEMY_ANI_RIGHT_DOWN			15
#define ENEMY_ANI_LEFT_WING				16
#define ENEMY_ANI_RIGHT_WING			17
#define ENEMY_ANI_WING_JUMP				18
#define ENEMY_ANI_MOVE_LEFT_UP			19
#define ENEMY_ANI_MOVE_RIGHT_UP			20
#define ENEMY_ANI_MOVE_LEFT_DOWN		21
#define ENEMY_ANI_MOVE_RIGHT_DOWN		22
#define ENEMY_ANI_DIE_STOMP				80
#define ENEMY_ANI_DIE_HIT_LEFT			81
#define ENEMY_ANI_DIE_HIT_RIGHT			82

enum eType
{
	//None
	NONE = 0,

	//Block
	BLOCK = 01,
	BACKGROUND = 02,
	GROUP = 03,
	PLATFORM = 12,

	//Mario 
	PLAYER = 04,
	PLAYER_UNTOUCHABLE = 05,

	//Enemy Mob
	ENEMY = 06,
	ENEMY_MOB_DIE = 07,

	//Active Block
	BRICK = 10,
	QUESTION = 11,

	//Bullet
	MARIO_BULLET = 50,
	ENEMY_BULLET = 51
	


	
};

class Global
{
public:
	static int Sign(float x) { return (x > 0) - (x < 0); }

};

typedef Global* GLOBAL;
