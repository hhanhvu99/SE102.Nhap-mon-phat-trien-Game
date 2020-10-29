#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "GameEngine.h"
#include "Texture.h"

Texture* Texture::__instance = NULL;

#define ID_TEX_BBOX -1

Texture::Texture()
{

}

Texture* Texture::GetInstance()
{
	if (__instance == NULL) __instance = new Texture();
	return __instance;
}

void Texture::Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texture;
	
	HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = GameEngine::GetInstance()->GetDirect3DDevice();

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		filePath,						    // Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,					// Transparent color
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] CreateTextureFromFile failed. File: %s\n", filePath);
		return;
	}

	textures[id] = texture;

	DebugOut(L"[INFO] Texture loaded Ok: %s \n", filePath);
}

LPDIRECT3DTEXTURE9 Texture::Get(unsigned int i)
{
	return textures[i];
}

void Texture::Clear()
{
	for (auto x : textures)
	{
		LPDIRECT3DTEXTURE9 tex = x.second;
		if (tex != NULL) tex->Release();
	}

	textures.clear();
}


