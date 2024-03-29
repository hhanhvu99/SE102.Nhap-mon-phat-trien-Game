﻿#include <iostream>
#include <fstream>
#include <cmath>

#include "debug.h"
#include "GameEngine.h"

GameEngine* GameEngine::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
void GameEngine::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	screen_height = r.bottom + 1;
	screen_width = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	OutputDebugString(L"[INFO] InitGame done;\n");
}

/*
	Utility function to wrap LPD3DXSPRITE::Draw
*/

void GameEngine::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, D3DCOLOR color, float angle, float offsetX, float offsetY)
{
	D3DXVECTOR3 p(trunc(x - cam_x + offsetX), trunc(y - cam_y + offsetY), 0);
	//D3DXVECTOR3 p(x - cam_x, y - cam_y, 0);
	D3DXMATRIX anhGoc;
	D3DXMATRIX scale;
	D3DXMATRIX anhCuoi;

	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	if (allowColor == false)
		this->color = color;

	//DebugOut(L"Direction: %d\n", angle);

	//Góc bình thường
	if (angle == 0.0f)
	{
		spriteHandler->Draw(texture, &r, NULL, &p, this->color);
	}
	//Góc lật ngược (trái thành phải)
	else if (angle == 180.0f)
	{
		spriteHandler->GetTransform(&anhGoc);
		D3DXVECTOR2 center(p.x + (right-left)/2 , p.y);
		D3DXMatrixTransformation2D(&scale, &center, 0.0f, &D3DXVECTOR2(-1.0f, 1.0f), NULL, 0.0f, NULL);
		anhCuoi = anhGoc * scale;

		spriteHandler->SetTransform(&anhCuoi);
		spriteHandler->Draw(texture, &r, NULL, &p, this->color);
		spriteHandler->SetTransform(&anhGoc);
	}
	//Góc úp ngược
	else if (angle == 90.0f)
	{
		spriteHandler->GetTransform(&anhGoc);
		D3DXVECTOR2 center(p.x + (right - left) / 2, p.y + (right - left) / 2);
		D3DXMatrixTransformation2D(&scale, NULL, 0.0f, NULL, &center, D3DXToRadian(180.0f), NULL);
		anhCuoi = anhGoc * scale;

		spriteHandler->SetTransform(&anhCuoi);
		spriteHandler->Draw(texture, &r, NULL, &p, this->color);
		spriteHandler->SetTransform(&anhGoc);
	}
	//Góc lật ngược trái sang phải và úp ngược
	else if (angle == 90.1f)
	{
		spriteHandler->GetTransform(&anhGoc);
		D3DXVECTOR2 center(p.x + (right - left) / 2, p.y);
		D3DXVECTOR2 center2(p.x + (right - left) / 2, p.y + (right - left) / 2);
		D3DXMatrixTransformation2D(&scale, &center, 0.0f, &D3DXVECTOR2(-1.0f, 1.0f), &center2, D3DXToRadian(180.0f), NULL);
		anhCuoi = anhGoc * scale;

		spriteHandler->SetTransform(&anhCuoi);
		spriteHandler->Draw(texture, &r, NULL, &p, this->color);
		spriteHandler->SetTransform(&anhGoc);
	}
	//Góc xoay bình thường
	else if (angle != 0.0f)
	{
		spriteHandler->GetTransform(&anhGoc);
		D3DXVECTOR2 center(p.x + (right - left) / 2, p.y + (right - left) / 2);
		D3DXMatrixTransformation2D(&scale, NULL, 0.0f, NULL, &center, D3DXToRadian(angle), NULL);
		anhCuoi = anhGoc * scale;

		spriteHandler->SetTransform(&anhCuoi);
		spriteHandler->Draw(texture, &r, NULL, &p, this->color);
		spriteHandler->SetTransform(&anhGoc);
	}
	
}

int GameEngine::IsKeyDown(int KeyCode)
{
	int result = (keyStates[KeyCode] & 0x80) > 0;
	if (result == 1)
		lastKeyPressed = KeyCode;
	return result;
}

void GameEngine::InitKeyboard()
{
	HRESULT
		hr = DirectInput8Create
		(
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DirectInput8Create failed!\n");
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] CreateDevice failed!\n");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
		return;
	}

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
}

void GameEngine::ProcessKeyboard()
{
	HRESULT hr;

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	keyHandler->KeyState((BYTE*)&keyStates);



	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}

GameEngine::~GameEngine()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}

/*
	Standard sweptAABB implementation
	Source: GameDev.net
*/
void GameEngine::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny,
	float& dx_entry, float& dx_exit, float& tx_entry, float& tx_exit,
	float& dy_entry, float& dy_exit, float& ty_entry, float& ty_exit,
	float& t_entry, float& t_exit)
{
	//Dùng cho debug
	//float dx_entry, dx_exit, tx_entry, tx_exit;
	//float dy_entry, dy_exit, ty_entry, ty_exit;

	//float t_entry;
	//float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}

	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}

/*
	Camera đi theo Mario
	Check bên trong Mario. Khi chạm vào cạnh camera sẽ tự di chuyển theo hướng chạm
*/

void GameEngine::UpdateCamPos(float marioX, float marioY)
{
	//left, top, right, bottom tương ứng với 4 cạnh bao bọc xung quanh Mario
	float left = cam_x + CAMERA_POSITION_LEFT;
	float top = cam_y + CAMERA_POSITION_TOP;
	float right = cam_x + CAMERA_POSITION_RIGHT;
	float bottom = cam_y + CAMERA_POSITION_BOTTOM;

	if (marioX < left)
	{
		cam_x = marioX - CAMERA_POSITION_LEFT;
	}
	else if (marioX > right)
	{
		cam_x = marioX - CAMERA_POSITION_RIGHT;
	}

	if (marioY < top)
	{
		cam_y = marioY - CAMERA_POSITION_TOP;
	}
	else if (marioY > bottom)
	{
		cam_y = marioY - CAMERA_POSITION_BOTTOM;
	}

	//Check bên ngoài Mario. Nếu chạm vào cạnh của scene thì camera sẽ dừng tại cạnh đó
	//cam_x, cam_y, outSideRight, outSideBottom tương ứng với 4 cạnh của màn hình chơi
	float outSideRight = cam_x + SCREEN_WIDTH;
	float outSideBottom = cam_y + SCREEN_HEIGHT;

	if (cam_x < 0.0f)
	{
		cam_x = 0.0f;
	}
	else if (outSideRight > Global::GetInstance()->screenWidth)
	{
		cam_x = Global::GetInstance()->screenWidth - SCREEN_WIDTH;
	}

	if (cam_y < 0.0f)
	{
		cam_y = 0.0f;
	}
	else if (outSideBottom > Global::GetInstance()->screenHeight + 35.0f)
	{
		cam_y = Global::GetInstance()->screenHeight - SCREEN_HEIGHT + 35.0f;
	}

	//Làm cho HUD không bị dịch chuyển
	cam_x = trunc(cam_x);
	cam_y = trunc(cam_y);

	//Reset lại vị trí của Mario ở phần SlideCamPos
	camTemp_x = cam_x;
}

/*
	Camera trượt không theo Mario
	Khi chạm tới cạnh phải của scene thì camera tự dừng
*/

void GameEngine::SlideCamPos(float speed, DWORD dt)
{
	if (cam_x + SCREEN_WIDTH < Global::GetInstance()->screenWidth)
		camTemp_x += speed * dt;

	//Làm cho HUD không bị dịch chuyển
	cam_x = trunc(camTemp_x);
	cam_y = trunc(cam_y);

}

	GameEngine* GameEngine::GetInstance()
{
	if (__instance == NULL) __instance = new GameEngine();
	return __instance;
}


