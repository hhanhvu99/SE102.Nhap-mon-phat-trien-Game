#pragma once

#include <d3d9.h>
#include <d3dx9.h>

//Basic Info
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224 
#define MAX_FRAME_RATE 120
#define STANDARD_SIZE 16

//Define Draw Order
#define BACKGROUND_DRAW_ORDER			0
#define ENEMY_ENTITY_PLANT_DRAW_ORDER	1
#define BLOCK_DRAW_ORDER				2
#define ACTIVE_BLOCK_DRAW_ORDER			3
#define PLAYER_DRAW_ORDER				4
#define ENEMY_ENTITY_DRAW_ORDER			5
#define BULLET_DRAW_ORDER				6
#define HUD_FRAME_DRAW_ORDER			7
#define HUD_TEXT_DRAW_ORDER				8

//Scene ID
#define SCENE_INTRO						1
#define SCENE_WORLD_1					2
#define SCENE_WORLD_1_1					3
#define SCENE_WORLD_1_1_BONUS			4

/*Define Active Block*/
//Animation
#define ACTIVE_BLOCK					1134
#define BLOCK_ANI_TIME					150

#define BRICK_SHINY_ANI					91
#define QUESTION_BLOCK_ANI				251
#define QUESTION_BLOCK_ANI_HIT			255

#define P_BLOCK_ANI						110
#define P_BLOCK_ANI_STOMP				113

//State
//Object
#define ACTIVE_BLOCK_STATE_HIT			3

#define BRICK_SHINY_STATE_DESTROY		0
#define BRICK_SHINY_STATE_NORMAL		1
#define BRICK_SHINY_STATE_MOVING		2
#define BRICK_SHINY_STATE_MULTIPLE		3
#define BRICK_SHINY_STATE_HIT			4

#define QUESTION_BLOCK_STATE_NORMAL		1
#define QUESTION_BLOCK_STATE_MOVING		2
#define QUESTION_BLOCK_STATE_HIT		3

#define GOAL_STATE_IDLE					0
#define GOAL_STATE_NORMAL				1
#define GOAL_STATE_HIT					2

#define P_BLOCK_STATE_IDLE				0
#define P_BLOCK_STATE_NORMAL			1
#define P_BLOCK_STATE_MOVING			2
#define P_BLOCK_STATE_HIT				3
#define P_BLOCK_STATE_STOMP				4

#define MOVING_SPEED					0.3f
#define DEFLECT_SPEED					0.05f
#define BOUNDARY						0.8f

#define BLOCK_MOVING_TIME				20

//Item
#define ITEM_STATE_IDLE					0
#define ITEM_STATE_SHOW					1
#define ITEM_STATE_MOVING				2
#define ITEM_STATE_HIT					3
#define ITEM_STATE_DROP					4

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

//Auto move
extern int CHOOSE;

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

//Level Luigi
#define LUIGI_LEVEL_BIG					2200

//Level Mario Map
#define MARIO_MAP_LEVEL					2000

////////////////
//Mario Suffix//
////////////////
//Rolling Map
#define MARIO_ANI_ROLLING			5
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
#define MARIO_ANI_HOLD_L_TO_R		89
#define MARIO_ANI_HOLD_R_TO_L		90
//Transition
#define MARIO_ANI_TRAN_LEFT_UP		1279
#define MARIO_ANI_TRAN_RIGHT_UP		1280
#define MARIO_ANI_TRAN_LEFT_DOWN	1291
#define MARIO_ANI_TRAN_RIGHT_DOWN	1292
//Die
#define MARIO_ANI_DIE				1800
//Icon Map
#define MARIO_SPRITE_ICON_MAP		3001

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
#define BULLET_EFFECT_RUBBISH			51000

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
#define ENEMY_BEETLE					310000
#define ENEMY_KOOPAS_MENU				320000
#define ENEMY_KOOPAS_MENU_SPECIAL		330000
#define ENEMY_KOOPAS_RED				400000
#define ENEMY_PIRANHA_GREEN				500000
#define ENEMY_VENUS_GREEN				600000
#define ENEMY_VENUS_RED					700000

//Map
#define ENEMY_TROOP						515

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
#define ENEMY_ANI_IDLE					23
#define ENEMY_ANI_DIE_STOMP				80
#define ENEMY_ANI_DIE_HIT_LEFT			81
#define ENEMY_ANI_DIE_HIT_RIGHT			82

////////
//Item//
////////
#define ITEM_ID							6765

///Type
#define ITEM_MUSHROOM_RED				1000000
#define ITEM_MUSHROOM_GREEN				1100000
#define ITEM_SUPER_LEAF					2000000
#define ITEM_LEAF						2100000
#define ITEM_SUPER_STAR					3000000
#define ITEM_PWING						4000000
#define ITEM_FIRE_FLOWER				5000000
#define ITEM_COIN						6000000
#define ITEM_COIN_ID					270

///////////////
//Item Suffix//
///////////////
#define ITEM_ANI_LEFT					1
#define ITEM_ANI_RIGHT					2

///////
//HUD//
///////
#define HUD_ID							10000000

//Bonus Point
#define HUD_BONUS_POINT_100				100
#define HUD_BONUS_POINT_200				200
#define HUD_BONUS_POINT_400				400
#define HUD_BONUS_POINT_800				800
#define HUD_BONUS_POINT_1000			1000
#define HUD_BONUS_POINT_2000			2000
#define HUD_BONUS_POINT_4000			4000
#define HUD_BONUS_POINT_8000			8000
#define HUD_BONUS_POINT_UP				10000

//Icon
#define HUD_ICON_MARIO					20000
#define HUD_ICON_ARROW_BRIGHT			20001
#define HUD_ICON_BUTTON_BRIGHT			20002
#define HUD_ICON_ARROW_DARK				20003
#define HUD_ICON_BUTTON_DARK			20004

//Main Frame
#define HUD_FRAME						30000
#define HUD_ITEM_EMPTY					30001
#define HUD_ITEM_MUSHROOM				30002
#define HUD_ITEM_FLOWER					30003
#define HUD_ITEM_STAR					30004

//Game Over
#define HUD_GAMEOVER					40001
//Start Screen
#define HUD_START						40002					

////////
//Menu//
////////
#define MENU							7976

#define MENU_TITLE_TEXT					323
#define MENU_TITLE_TEXT_BRIGHT			324
#define MENU_ARROW						332

#define MENU_ANI_NUMBER					328

//Menu Draw Order
#define MENU_DRAW_ORDER_RIBBON			1
#define MENU_DRAW_ORDER_RIBBON_BG		2
#define MENU_DRAW_ORDER_TREE			3
#define MENU_DRAW_ORDER_TITLE			4
#define MENU_DRAW_ORDER_ENEMY			5
#define MENU_DRAW_ORDER_PLAYER			6
#define MENU_DRAW_ORDER_ITEM			7

//Menu State
#define MENU_STATE_SKIP					100
#define MENU_STATE_OPTION				200
#define MENU_STATE_CHOOSE				300

/////////////
//World Map//
/////////////
#define MAP_ANI_ID						2124
#define MAP_ID							500

#define MAP_TREE_ID						506
#define MAP_POPUP_ID					508
#define MAP_MARIO_FINISHED				527

//Map Animation
#define MAP_TREE_ANIMATION_ID			505
#define MAP_ENEMY_TROOP_ANI_LEFT		515
#define MAP_ENEMY_TROOP_ANI_RIGHT		615
#define MAP_ENEMY_TROOP_ANI_DIE			517

//Info
#define MAP_PATH_START					0
#define MAP_PATH_STAGE_ONE				1
#define MAP_PATH_STAGE_TWO				2
#define MAP_PATH_STAGE_THREE			3
#define MAP_PATH_STAGE_FOUR				4
#define MAP_PATH_STAGE_FIVE				5
#define MAP_PATH_STAGE_SIX				6
#define MAP_PATH_GAMBLE_ONE				7
#define MAP_PATH_GAMBLE_TWO				8
#define MAP_PATH_CASTLE_MID				9
#define MAP_PATH_CASTLE_TOAST			10
#define MAP_PATH_ROAD					11
#define MAP_PATH_CASTLE					12

//Draw Order
#define MAP_DRAW_ORDER_BACKGROUND		1
#define MAP_DRAW_ORDER_POPUP			2
#define MAP_DRAW_ORDER_ENEMY			3
#define MAP_DRAW_ORDER_PLAYER			4

//State
#define MAP_STATE_MOVE_UP				100
#define MAP_STATE_MOVE_DOWN				200
#define MAP_STATE_MOVE_LEFT				300
#define MAP_STATE_MOVE_RIGHT			400
#define MAP_STATE_RESET					500
#define MAP_STATE_RESTART				600
#define MAP_STATE_SELECT				700
#define MAP_STATE_CHOOSE				800
#define SCENE_STATE_SWITCH				900
#define SCENE_STATE_SWITCH_NEXT			901
#define SCENE_STATE_MAP_TO_STAGE		902
#define SCENE_STATE_STAGE_TO_MAP		903
#define SCENE_STATE_STILL_SWITCH        1000
	
////////////
//End Goal//
////////////
#define	END_GOAL_ID						6162

#define END_GOAL_SPRITE_ID				334
#define END_GOAL_SQUARE					601

#define END_GOAL_ANI_ID					600
#define END_GOAL_TEXT_1					612
#define END_GOAL_TEXT_2					613

#define END_GOAL_MUSHROOM				2
#define END_GOAL_FLOWER					3
#define END_GOAL_STAR					4

#define MUSHROOM_ANI_GOAL				612
#define FLOWER_ANI_GOAL					613
#define STAR_ANI_GOAL					614

//////////
//Camera//
//////////
#define CAMERA_POSITION_LEFT			96.0f
#define CAMERA_POSITION_TOP				32.0f
#define CAMERA_POSITION_RIGHT			144.0f
#define CAMERA_POSITION_BOTTOM			128.0f

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
	P_BLOCK = 13,

	//Item
	ITEM = 14,

	//End Goal
	GOAL = 15,

	//Teleport Type
	TELEPORT_SCENE = 20,
	TELEPORT_MAP = 21,
	TELEPORT_SCENE_TO_MAP = 22,

	//Bullet
	MARIO_BULLET = 50,
	ENEMY_BULLET = 51,

	//HUD
	HUD_MAIN_FRAME = 100,
	HUD_PLAYER_ICON = 101,
	HUD_PLAYER_LIVE = 102,
	HUD_WORLD = 103,
	HUD_SPEEDOMETER = 104,
	HUD_POINT = 105,
	HUD_MONEY = 106,
	HUD_TIME = 107,
	HUD_CARD_ONE = 108,
	HUD_CARD_TWO = 109,
	HUD_CARD_THREE = 110,
	HUD_BUBBLE = 111,
	HUD_OBJECT_IMAGE = 112,
	HUD_OBJECT_TEXT	= 113,
	HUD_CUSTOM = 114,

	//Menu
	MENU_TITLE = 200,

	//Map
	MAP_TREE = 300,
	MAP_POPUP = 301,
	MAP_START = 302,
	MAP_GAMEOVER = 303

};

class Global
{
	static Global* __instance;

public:
	//Screen width and height
	float screenWidth, screenHeight;

	//Mario 
	int level;
	int cardGet;
	bool die = false;
	bool finished = false;
	bool allowCountTime = false;

	//HUD
	int live, player, world, point, time, money;
	int cardOne, cardTwo, cardThree;
	float frameHUD_x, frameHUD_y;
	int speed;
	int currentCardEmpty;

	//Switch Scene
	bool allowSwitch;

	D3DCOLOR background_color = D3DCOLOR_XRGB(255, 255, 255);

	void Setup()
	{
		this->live = 4;
		this->player = HUD_ICON_MARIO;
		this->world = 1;
		this->speed = 0;
		this->point = 0;
		this->money = 0;
		this->time = 0;
		this->cardOne = HUD_ITEM_EMPTY;
		this->cardTwo = HUD_ITEM_EMPTY;
		this->cardThree = HUD_ITEM_EMPTY;
		this->currentCardEmpty = 1;
	}

	//Map
	int currentX, currentY;

	static int Sign(float x) { return (x > 0) - (x < 0); }
	static int TwoDimension_To_OneDimension(int i, int j, int rowLength) { return (i * rowLength) + j; }
	static Global* GetInstance();

};

typedef Global* GLOBAL;
