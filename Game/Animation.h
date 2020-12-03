#pragma once

#include "SpriteManager.h"

class AnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	AnimationFrame(LPSPRITE sprite, DWORD time)
	{ 
		this->sprite = sprite; 
		this->time = time; 
	}
	DWORD GetTime() { return time; }
	void SetTime(DWORD time) { this->time = time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef AnimationFrame* LPANIMATION_FRAME;

class Animation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	int id;
	vector<LPANIMATION_FRAME> frames;
public:
	Animation(int time = 100)
	{ 
		this->defaultTime = time;
		lastFrameTime = -1; 
		currentFrame = -1; 
	}
	int GetCurrentFrame() { return this->currentFrame; }
	int GetSize() { return frames.size(); }
	void Reset() { lastFrameTime = GetTickCount(); }
	void Add(int spriteId, DWORD time = 100);
	void Render(float x, float y, D3DCOLOR color = D3DCOLOR_ARGB(255,255,255,255));
	bool RenderOnce(float x, float y, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void Render(float x, float y, float rotate, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
	void SetTime(DWORD time);
	DWORD GetTime() { return this->defaultTime; }

	~Animation();
};

typedef Animation* LPANIMATION;

class AnimationSet
{
	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void SetTime(DWORD time);
	void SetTime(int id, DWORD time);
	void Clear();
};

typedef AnimationSet* LPANIMATION_SET;
