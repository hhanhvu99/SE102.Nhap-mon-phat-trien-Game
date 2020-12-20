#include "GroupObject.h"

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
	float currentX, currentY;
	this->x += x;
	this->y += y;


	for (LPGAMEOBJECT object : group)
	{
		object->GetPosition(currentX, currentY);
		object->SetPosition(currentX + x, currentY + y);
	}
}

void GroupObject::SetGroupPos(float x, float y)
{
	float currentX, currentY;
	float dx = this->x - x;
	float dy = this->y - y;
	this->x = x;
	this->y = y;


	for (LPGAMEOBJECT object : group)
	{
		object->GetPosition(currentX, currentY);
		object->SetPosition(currentX + dx, currentY + dy);
	}
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
	for (LPGAMEOBJECT object : group)
	{
		object->Update(dt);
	}

}

void GroupObject::Render()
{
	for (LPGAMEOBJECT object : group)
	{
		object->Render();
	}

	//RenderBoundingBox();
}

GroupObject::~GroupObject()
{
	for (LPGAMEOBJECT object : group)
	{
		delete object;
		object = NULL;
	}

	group.clear();
}
