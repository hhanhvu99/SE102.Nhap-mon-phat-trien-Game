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

	this->indexX = left;
	this->indexY = top;

}

GroupObject::GroupObject()
{
	left = top = 70000;
	right = bottom = -1;
	vx = vy = 0;

}

void GroupObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = this->left;
	top = this->top;
	right = this->right;
	bottom = this->bottom;

}

void GroupObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (LPGAMEOBJECT object : group)
	{
		object->SetSpeed(vx, vy);
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
