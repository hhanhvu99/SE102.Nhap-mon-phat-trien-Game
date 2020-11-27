#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>

#include "SceneManager.h"

#define KEYBOARD_BUFFER_SIZE 1024
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


using namespace std;

class GameEngine
{
	static GameEngine* __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;  

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height;

	int lastKeyPressed;

public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(	float x, 
				float y, 
				LPDIRECT3DTEXTURE9 texture, 
				int left, int top, int right, int bottom, 
				D3DCOLOR color = D3DCOLOR_ARGB(255,255,255,255),
				float angle = 0,
				float offset_X = 0,
				float offset_Y = 0
	);

	int IsKeyDown(int KeyCode);
	int GetLastKey() { return lastKeyPressed; }
	void ProcessKeyboard();

	LPSCENE GetCurrentScene() { return SceneManager::GetInstance()->GetCurrentScene(); }

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	void GetCamPos(float &x, float &y) { x = cam_x ; y = cam_y ; }

	static GameEngine* GetInstance();

	~GameEngine();
};