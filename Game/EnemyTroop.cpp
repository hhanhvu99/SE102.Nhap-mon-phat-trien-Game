#include "EnemyTroop.h"

EnemyTroop::EnemyTroop(int indexX, int indexY, int mobType) : GameObject()
{
	this->x = indexX * STANDARD_SIZE;
	this->y = indexY * STANDARD_SIZE;
	oldX = this->x;
	oldY = this->y;
	direction = 1;

	this->Add();
}

void EnemyTroop::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Add(this);

}

void EnemyTroop::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Destroy(this);

}

void EnemyTroop::Remove()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Remove(this);
}

void EnemyTroop::MoveTo(float x, float y)
{
	newX = x;
	newY = y;
	isMoving = true;
	oldDist = 9999999.9f;

	vectorX = newX - this->x;
	vectorY = newY - this->y;
	magnitude = float(sqrt(vectorX * vectorX + vectorY * vectorY));

	vx = vectorX / magnitude * ENEMY_TROOP_MOVE_SPEED;
	vy = vectorY / magnitude * ENEMY_TROOP_MOVE_SPEED;
}

void EnemyTroop::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
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
				oldX = x = newX;
				oldY = y = newY;
				vx = 0;
				vy = 0;
			}
			else
			{
				oldDist = newDist;
			}

		}
		else
		{
			if (this->x < oldX - ENEMY_TROOP_BOUNDARY)
				direction = 1;
			else if (this->x > oldX + ENEMY_TROOP_BOUNDARY)
				direction = -1;

			vx = direction * ENEMY_TROOP_MOVE_SPEED;
		}
		
	}

}

void EnemyTroop::Render()
{
	if (PAUSE == false)
	{
		int ani = -1;

		if (direction > 0)
			ani = MAP_ENEMY_TROOP_ANI_RIGHT;
		else
			ani = MAP_ENEMY_TROOP_ANI_LEFT;

		animation_set->Get(ani)->Render(x, y);
	}
	
}
