#pragma once

#include "GameObject.h"
#include "Mario.h"

#define GROUP_MOVING_X				0.02f
#define GROUP_MOVING_Y				0.0001f
#define GROUP_MAX_FALLING			0.15f

#define ENTITY_SAFE_MOVING_RANGE	60.0f

class GroupObject : public GameObject
{
	vector<LPGAMEOBJECT> group;
	float left, top, right, bottom;
	float camPosX, camPosY;
	bool dropping = false;

public:
	GroupObject();

	void Add(LPGAMEOBJECT& gameObject);
	void Move(float x, float y);
	void SetGroupPos(float x, float y);
	void Destroy_Group();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void Destroy();
	virtual void SetState(int state);

	~GroupObject();

};

