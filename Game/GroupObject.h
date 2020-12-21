#pragma once

#include "GameObject.h"

class GroupObject : public GameObject
{
	vector<LPGAMEOBJECT> group;
	float left, top, right, bottom;


public:
	GroupObject();

	void Add(LPGAMEOBJECT& gameObject);
	void Move(float x, float y);
	void SetGroupPos(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	~GroupObject();

};

