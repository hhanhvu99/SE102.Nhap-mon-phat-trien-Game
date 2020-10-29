#pragma once

#include "GameObject.h"

class GroupObject : public GameObject
{
	vector<LPGAMEOBJECT> group;
	float vx, vy;
	float left, top, right, bottom;


public:
	void Add(LPGAMEOBJECT& gameObject);
	void Move(float vx, float vy) { this->vx = vx; this->vy = vy; }

	GroupObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	~GroupObject();

};

