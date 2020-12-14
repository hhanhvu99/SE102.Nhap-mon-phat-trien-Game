#include "Global.h"

int CHOOSE = 3;
bool PAUSE = false;
Global* Global::__instance = NULL;

Global* Global::GetInstance()
{
	if (__instance == NULL) __instance = new Global();
	return __instance;
}
