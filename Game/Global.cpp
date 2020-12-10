#include "Global.h"

bool PAUSE = false;
bool AUTO = false;
Global* Global::__instance = NULL;

Global* Global::GetInstance()
{
	if (__instance == NULL) __instance = new Global();
	return __instance;
}
