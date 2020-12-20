#include <string>
#include <cmath>

#include "MarioMap.h"

MarioMap::MarioMap(float x, float y) : GameObject()
{
	global = Global::GetInstance();
	this->x = x;
	this->y = y;
	this->width = int(STANDARD_SIZE);
	this->height = int(STANDARD_SIZE);
}

void MarioMap::MoveTo(float x, float y)
{
	newX = x;
	newY = y;
	isMoving = true;
	oldDist = 9999999.9f;

	vectorX = newX - this->x;
	vectorY = newY - this->y;
	magnitude = float(sqrt(vectorX * vectorX + vectorY * vectorY));

	vx = vectorX / magnitude * MARIO_MAP_MOVE_SPEED;
	vy = vectorY / magnitude * MARIO_MAP_MOVE_SPEED;
}

void MarioMap::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (PAUSE == false)
	{
		GameObject::Update(dt);
		x += dx;
		y += dy;

		if (isMoving)
		{
			float distX = pow((x - newX), 2);
			float distY = pow((y - newY), 2);
			float newDist = distX + distY;

			if (newDist > oldDist)
			{
				isMoving = false;
				isRolling = false;
				x = newX;
				y = newY;
				vx = 0;
				vy = 0;
			}
			else
			{
				oldDist = newDist;
			}

		}
	}
	
}

void MarioMap::Render()
{
	if (PAUSE == false)
	{
		int ani = -1;

		if (isRolling)
			ani = global->level + MARIO_MAP_LEVEL + MARIO_ANI_ROLLING;
		else
			ani = global->level + MARIO_MAP_LEVEL + MARIO_ANI_IDLE_LEFT;

		animation_set->Get(ani)->Render(x, y);
	}
	
}

void MarioMap::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case MARIO_MAP_STATE_ROLLING:
		isRolling = true;
		break;
	default:
		break;
	}

}
