#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "SpriteManager.h"
#include "AnimationManager.h"
#include "Texture.h"
#include "GameEngine.h"
#include "Global.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class GameObject;
typedef GameObject* LPGAMEOBJECT;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;
struct CollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};


class GameObject
{
protected:
	float x;
	float y;

	float vx;
	float vy;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	int indexX;
	int indexY;

	int width;
	int height;

	int direction;
	int state;

	int draw_order;

	eType type;
	DWORD dt;

	bool touchGround;

	LPANIMATION_SET animation_set;
	LPSPRITE sprite;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void SetIndex(int x, int y) { this->indexX = x; this->indexY = y; }
	void SetDrawOrder(int order) { this->draw_order = order; }
	void SetDirection(int direction) { this->direction = direction; }

	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	void GetIndex(int& x, int& y) { x = this->indexX; y = this->indexY; }
	void GetDirection(int& direction) { direction = this->direction; }
	int GetDrawOrder() { return this->draw_order; }

	int GetState() { return this->state; }
	void SetType(eType type) { this->type = type; }
	eType GetType() { return this->type; }

	void SetAnimationSet(LPANIMATION_SET ani_set) { this->animation_set = ani_set; }
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }
	void RenderBoundingBox();


	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents, vector<eType> exclude = {});
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	GameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }


	~GameObject();
};