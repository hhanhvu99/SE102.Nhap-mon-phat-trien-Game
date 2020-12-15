#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <fstream>

#include "Global.h"
#include "debug.h"
#include "GameObject.h"
#include "GameEngine.h"

#include "Texture.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"

#include "TestScene.h"
#include "Intro.h"
#include "WorldMap.h"
#include "World1_1.h"

#include "Mario.h"
#include "Keyboard.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"01 - Skeleton"

#define BRICK_TEXTURE_PATH L"brick.png"
#define MARIO_TEXTURE_PATH L"mario.png"


using namespace std;

GameEngine* gameEngine;

//vector<LPGAMEOBJECT> objects;  

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all GameEngine resources, Sprites and Animations. 
*/
void LoadResources()
{
	Texture* texture = Texture::GetInstance();

	texture->Add(1, L"Asset\\Block\\SpriteSheet.png", NULL);
	texture->Add(2, L"Asset\\Character\\Mario\\MarioSheet.png", D3DCOLOR_XRGB(68, 145, 190));
	texture->Add(3, L"Asset\\Character\\Enemy\\MobSheet.png", NULL);
	texture->Add(4, L"Asset\\Item\\ItemSheet.png", NULL);
	texture->Add(5, L"Asset\\HUD\\HUD_Sheet.png", NULL);
	texture->Add(6, L"Asset\\Menu\\MenuSheet.png", NULL);
	texture->Add(7, L"Asset\\Map\\SpriteSheetMap.png", NULL);
	texture->Add(ID_TEX_BBOX, L"Asset\\Debug\\BoundingBox.png", NULL);

	SpriteManager* spriteData = SpriteManager::GetInstance();
	AnimationManager* aniData = AnimationManager::GetInstance();

	LPDIRECT3DTEXTURE9 sheet = texture->Get(1);

	int size = 16;
	int beginX, beginY, endX, endY, padding = 2;
	int width = 19;
	int height = 18;
	int id = 0;

	//Load sprite của Map
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			beginX = size * i + i;
			beginY = size * j + j;
			endX = beginX + size;
			endY = beginY + size;

			spriteData->Add(id, beginX, beginY, endX, endY, sheet);
			++id;
		}
	}

	sheet = texture->Get(7);
	height = 5;
	width = 10;
	id = 0;
	//Load sprite của World Map
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			beginX = size * i + i;
			beginY = size * j + j;
			endX = beginX + size;
			endY = beginY + size;

			spriteData->Add(id + MAP_ID, beginX, beginY, endX, endY, sheet);
			++id;
		}
	}
	
	/*Load animaion cho ActiveBlock*/
	aniData->Load(L"Script\\Assets\\Animations\\ActiveBlockAnimations.txt");

	/*Load sprites và animations cho Mario*/

	sheet = texture->Get(2);

	//////////////////////
	//---Load sprites---//
	//////////////////////

	//--Bullet--//
	spriteData->Load(L"Script\\Assets\\Sprites\\BulletSprites.txt", sheet);

	//--Mario--//
	spriteData->Load(L"Script\\Assets\\Sprites\\MarioSprites.txt", sheet);

	//--Mob--//
	sheet = texture->Get(3);
	spriteData->Load(L"Script\\Assets\\Sprites\\EnemyMobSprites.txt", sheet);

	//--Item--//
	sheet = texture->Get(4);
	spriteData->Load(L"Script\\Assets\\Sprites\\ItemSprites.txt", sheet);

	//--HUD--//
	sheet = texture->Get(5);
	spriteData->Load(L"Script\\Assets\\Sprites\\HUD_Sprites.txt", sheet);

	//--Menu--//
	sheet = texture->Get(6);
	spriteData->Load(L"Script\\Assets\\Sprites\\MenuSprites.txt", sheet);

	//--World Map--//
	sheet = texture->Get(7);
	spriteData->Load(L"Script\\Assets\\Sprites\\MapSprites.txt", sheet);

	////////////////////////
	//---Load animation---//
	////////////////////////

	//--Mario--//
	aniData->Load(L"Script\\Assets\\Animations\\MarioAnimation.txt");

	//--Bullet--//
	aniData->Load(L"Script\\Assets\\Animations\\BulletAnimations.txt");

	//--Mob--//
	aniData->Load(L"Script\\Assets\\Animations\\EnemyAnimations.txt");

	//--Item--//
	aniData->Load(L"Script\\Assets\\Animations\\ItemAnimations.txt");

	//--Menu--//
	aniData->Load(L"Script\\Assets\\Animations\\MenuAnimation.txt");

	//--World Map--//
	aniData->Load(L"Script\\Assets\\Animations\\MapAnimation.txt");
	
	////////////////////
	//---Load Array---//
	////////////////////
	LPSCENE scene = new World1_1(SCENE_WORLD_1_1, L"Script\\Map\\TestMap.txt");
	scene->LoadBlock(L"Script\\Assets\\Block\\Block.txt");

	LPSCENE sceneBonus = new World1_1(SCENE_WORLD_1_1_BONUS, L"Script\\Map\\World1_1_Bonus.txt");
	sceneBonus->LoadBlock(L"Script\\Assets\\Block\\World1_1_Bonus.txt");

	LPSCENE intro = new Intro(SCENE_INTRO, L"Script\\Map\\Intro.txt");
	intro->LoadBlock(L"Script\\Assets\\Block\\Intro.txt");

	LPSCENE world = new WorldMap(SCENE_WORLD_1, L"Script\\Map\\WorldMap1.txt");
	world->LoadBlock(L"Script\\Assets\\Block\\WorldMap1.txt");

	LPSCENEMANAGER sceneManager = SceneManager::GetInstance();
	
	sceneManager->Load(scene);
	sceneManager->Load(sceneBonus);
	sceneManager->Load(intro);
	sceneManager->Load(world);
	sceneManager->SwitchScene(SCENE_INTRO);

	DebugOut(L"[INFO] Loading map successfully\n");
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
/*
	Update các GameObject
*/
void Update(DWORD dt)
{
	SceneManager::GetInstance()->GetCurrentScene()->Update(dt);
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = gameEngine->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = gameEngine->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = gameEngine->GetSpriteHandler();

	LPSCENEMANAGER scenes = SceneManager::GetInstance();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, Global::GetInstance()->background_color);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		// Render tất cả GameObject
		
		scenes->GetCurrentScene()->Render();


		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	RECT wnd = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT dt;
	GetClientRect(GetDesktopWindow(), &dt);
	AdjustWindowRect(&wnd, WS_OVERLAPPEDWINDOW, FALSE);
	int w = (wnd.right - wnd.left) - 1;
	int h = (wnd.bottom - wnd.top) - 1;
	int x = ((dt.right - dt.left) - w) / 2;
	int y = ((dt.bottom - dt.top) - h) / 2;

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			x,
			y,
			w,
			h,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD accumulator = 0;
	DWORD timePass = 0;
	const DWORD timeStep = 16;
	float tickPerFrame = 1000.0f / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Calculate the delta time (in seconds)
		DWORD now = GetTickCount();
		DWORD dt = now - frameStart;

		//DWORD now = GetTickCount();
		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		//DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			accumulator += dt;

			
			while (accumulator >= timeStep)
			{
				gameEngine->ProcessKeyboard();
				Update(timeStep);
				accumulator -= timeStep;
			}
			

			Render();
		}
		else
		{
			Sleep((DWORD)tickPerFrame - dt);
		}
		
		if (Global::GetInstance()->time > 0)
		{
			timePass += dt;

			if (timePass >= 1000)
			{
				Global::GetInstance()->time -= 1;
				timePass -= 1000;
			}
		}

	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	gameEngine = GameEngine::GetInstance();
	gameEngine->Init(hWnd);
	gameEngine->InitKeyboard();
	gameEngine->SetKeyHandler(Keyboard::GetInstance());

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}