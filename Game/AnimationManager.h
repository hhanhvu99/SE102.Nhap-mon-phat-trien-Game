#pragma once
#include "Animation.h"

/*
	Manage animation set database
*/
class AnimationManager
{
	static AnimationManager* __instance;
	bool first = false;
	bool timeChange = false;
	DWORD aniTime = 0;

	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	AnimationManager();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);
	void Clear();
	void SetTime(int id, DWORD time);

	void Load(LPCWSTR gameFile);
	int ParseCommon(string line, LPANIMATION ani);

	static AnimationManager* GetInstance();
};