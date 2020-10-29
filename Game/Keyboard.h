#pragma once
#include <Windows.h>

#include "KeyEventHandler.h"
#include "Mario.h"

class Mario;
class Keyboard : public KeyEventHandler
{
	static Keyboard* __instance;
	Mario* mario;

public:
	Keyboard() {};

	void SetKeyHandler(Mario* mario) { this->mario = mario; }
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode) {};

	static Keyboard* GetInstance();
};



