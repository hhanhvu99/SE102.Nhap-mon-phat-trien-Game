#include <fstream>

#include "AnimationManager.h"
#include "debug.h"

AnimationManager* AnimationManager::__instance = NULL;

AnimationManager::AnimationManager()
{

}

AnimationManager* AnimationManager::GetInstance()
{
	if (__instance == NULL) __instance = new AnimationManager();
	return __instance;
}

LPANIMATION_SET AnimationManager::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

void AnimationManager::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}

void AnimationManager::Clear()
{
	for (auto x : animation_sets)
	{
		LPANIMATION_SET ani = x.second;
		delete ani;
		ani = NULL;
	}

	animation_sets.clear();
}

void AnimationManager::SetTime(int id, DWORD time)
{
	LPANIMATION_SET animationSet = animation_sets[id];
	animationSet->SetTime(time);

}

#define MAX_GAME_LINE 1024

void AnimationManager::Load(LPCWSTR gameFile)
{
	ifstream f;
	f.open(gameFile);

	char str[MAX_GAME_LINE];
	bool begin = false;
	bool getID = false;

	LPANIMATION_SET ani_set = new AnimationSet();
	LPANIMATION ani = new Animation();
	int ani_set_id = 0;
	int ani_id = 0;
	int number = -1;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (number != -1) { ani_id = number; number = -1; }
		else if (number == -2)
		{
			DebugOut(L"[ERROR] Wrong animation format.\n");
			break;
		}

		if (begin && line[0] == '/') { begin = false; ani_set->Add(ani_id, ani); continue; }
		if (begin && line[0] == '-') { begin = true; first = true; ani_set->Add(ani_id, ani); ani = new Animation(); continue; }
		if (line[0] == '/') { continue; }
		if (line[0] == '-') { begin = true; first = true; ani = new Animation(); continue; }
		if (line == "[ID]") { getID = true; continue; }
		if (line == "End") 
		{ 
			ani_set->Add(ani_id, ani);
			this->Add(ani_set_id, ani_set); 
			break; 
		}

		if (getID) { ani_set_id = stoi(line); getID = false;  continue; }
		if (begin) { number = ParseCommon(line, ani); }


	}
	f.close();

	DebugOut(L"[INFO] Loading game animations : %s has been loaded successfully\n", gameFile);

}

int AnimationManager::ParseCommon(string line, LPANIMATION ani)
{
	vector<string> tokens = split(line, ",");
	vector<int> data;

	if (tokens.size() < 2)
		return -2;

	data.clear();

	for (int i = 0; i < 2; ++i)
		data.push_back(atoi(tokens[i].c_str()));

	if (data[1] == 0)
		return data[0];
	else
	{
		ani->Add(data[0]);
		if (first)
		{
			first = false;
			return data[0];
		}
		return -1;
	}


}
