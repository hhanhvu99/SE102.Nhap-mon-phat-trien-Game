#include "Animation.h"
#include "debug.h"

void Animation::Add(int spriteId, DWORD time)
{
	LPSPRITE sprite = SpriteManager::GetInstance()->Get(spriteId);
	id = spriteId;

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new AnimationFrame(sprite, time);
	frames.push_back(frame);
}

void Animation::Render(float x, float y, D3DCOLOR color)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		DWORD different = now - lastFrameTime;

		if (different > t && different <= t + 100)
		{
			++currentFrame;
			lastFrameTime = now;
			if (currentFrame == frames.size()) 
				currentFrame = 0;
		}
		else if (different > t + 100)
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, color);
}

bool Animation::RenderOnce(float x, float y, D3DCOLOR color)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		DWORD different = now - lastFrameTime;

		if (different > t && different <= t + 100)
		{
			++currentFrame;
			lastFrameTime = now;
			if (currentFrame == frames.size())
			{
				currentFrame = 0;
				return false;
			}
				
		}
		else if (different > t + 100)
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, color);
	return true;
}

void Animation::Render(float x, float y, float rotate, D3DCOLOR color)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		DWORD different = now - lastFrameTime;

		if (different > t && different <= t + 100)
		{
			++currentFrame;
			lastFrameTime = now;
			if (currentFrame == frames.size())
				currentFrame = 0;
		}
		else if (different > t + 100)
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, rotate, color);
}

void Animation::SetTime(DWORD time)
{
	this->defaultTime = time;

	for(auto x : frames)
	{
		x->SetTime(this->defaultTime);
	}
}

Animation::~Animation()
{
	for (auto x : frames)
	{
		delete x;
		x = NULL;
	}

	frames.clear();
}

void AnimationSet::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION AnimationSet::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void AnimationSet::SetTime(DWORD time)
{
	for (pair<int, LPANIMATION> x : animations)
	{
		x.second->SetTime(time);
	}

}

void AnimationSet::SetTime(int id, DWORD time)
{
	LPANIMATION animation = animations[id];
	animation->SetTime(time);

}

void AnimationSet::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
		ani = NULL;
	}

	animations.clear();
}
