#include "GroupObject.h"
#include "debug.h"

void GroupObject::Add(LPGAMEOBJECT& gameObject)
{
	group.push_back(gameObject);

	float tempL, tempT, tempR, tempB;
	gameObject->GetBoundingBox(tempL, tempT, tempR, tempB);

	if (left > tempL)
		left = tempL;
	if (top > tempT)
		top = tempT;
	if (right < tempR)
		right = tempR;
	if (bottom < tempB)
		bottom = tempB;

	this->x = left;
	this->y = top;

	this->width = int(right - left);
	this->height = int(bottom - top);

}

void GroupObject::Move(float x, float y)
{
	this->x = this->x + x;
	this->y = this->y + y;

	float currentOffsetX = 0.0f;
	for (LPGAMEOBJECT object : group)
	{
		object->SetPosition(this->x + currentOffsetX, this->y);
		currentOffsetX += STANDARD_SIZE;
	}
}

void GroupObject::SetGroupPos(float x, float y)
{
	float currentOffsetX = 0.0f;
	for (LPGAMEOBJECT object : group)
	{
		object->SetPosition(x + currentOffsetX, y);
		currentOffsetX += STANDARD_SIZE;
	}
}

void GroupObject::Destroy_Group()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	for (int cell : inCell)
	{
		if (Global::GetInstance()->cells.find(cell) != Global::GetInstance()->cells.end())
		{
			current->AddToCell(cell, this);
			return;
		}
	}

	current->Destroy(this);
}

GroupObject::GroupObject()
{
	left = top = 70000;
	right = bottom = -1;
	vx = vy = 0;

}

void GroupObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = this->x;
	top = this->y;
	right = left + width;
	bottom = top + height;

}

void GroupObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Check outside camera
	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);
	if (x < camPosX - ENTITY_SAFE_MOVING_RANGE || x > camPosX + SCREEN_WIDTH + ENTITY_SAFE_MOVING_RANGE ||
		y < camPosY - ENTITY_SAFE_MOVING_RANGE || y > camPosY + SCREEN_HEIGHT + ENTITY_SAFE_MOVING_RANGE)
	{
		vx = 0;
		vy = 0;
		return;
	}

	if (PAUSE == true)
	{
		return;
	}

	if (type == eType::GROUP_MOVING)
	{
		GameObject::Update(dt);
		x += dx;
		y += dy;

		if (dropping)
		{
			if (vy < GROUP_MAX_FALLING)
				vy += GROUP_MOVING_Y * dt;
			else
				vy = GROUP_MAX_FALLING;
			vx = 0;
		}
		else
		{
			vx = -GROUP_MOVING_X;
			vy = 0;
		}

		SetGroupPos(x, y);
	}
}

void GroupObject::Render()
{
	for (LPGAMEOBJECT object : group)
	{
		object->Render();
	}

	RenderBoundingBox();
}

void GroupObject::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	for (int cell : inCell)
	{
		if (Global::GetInstance()->cells.find(cell) != Global::GetInstance()->cells.end())
		{
			current->AddToCell(cell, this);
			return;
		}
	}

	current->Destroy(this);
}

void GroupObject::SetState(int state)
{
	GameObject::SetState(state);

	if (state == GROUP_MOVING_STATE_STOMP)
		dropping = true;

}

GroupObject::~GroupObject()
{
	int i, j;
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	GLOBAL global = Global::GetInstance();

	for (LPGAMEOBJECT object : group)
	{
		object->GetIndex(i, j);
		global->occupiedGroup.erase(global->TwoDimension_To_OneDimension(i, j, scene->width));
		delete object;
		object = NULL;
	}

	group.clear();
}
