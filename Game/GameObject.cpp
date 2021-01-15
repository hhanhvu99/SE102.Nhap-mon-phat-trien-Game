#include "GameObject.h"

#include <d3dx9.h>
#include <algorithm>

#include "debug.h"

GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
	indexX = indexY = -1;
	direction = 1;
	currentCell = -1;
}

void GameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT GameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;
	float t_entry, t_exit;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	GameEngine::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny, 
		dx_entry, dx_exit, tx_entry, tx_exit,
		dy_entry, dy_exit, ty_entry, ty_exit,
		t_entry, t_exit
	);

	/*
	if (coO->GetType() == eType::PLAYER && this->type == eType::ENEMY)
	{
		if (mt - sb < 3.0f && ml - sr < 3.0f && sl - mr < 3.0f)
		{
			DebugOut(L"-------------\n");
			DebugOut(L"ml: %f - mt: %f - mr: %f - mb: %f\n", ml, mt, mr, mb);
			DebugOut(L"sl: %f - st: %f - sr: %f - sb: %f\n", sl, st, sr, sb);
			DebugOut(L"t: %f - nx: %f - ny: %f\n", t, nx, ny);
			DebugOut(L"dx_entry: %f - dx_exit: %f - tx_entry: %f - tx_exit: %f\n", dx_entry, dx_exit, tx_entry, tx_exit);
			DebugOut(L"dy_entry: %f - dy_exit: %f - ty_entry: %f - ty_exit: %f\n", dy_entry, dy_exit, ty_entry, ty_exit);
			DebugOut(L"t_entry: %f - t_exit: %f\n", t_entry, t_exit);
			DebugOut(L"Enemy dx: %f - Enemy dy: %f - Player dx: %f - Player dy: %f\n", dx, dy, sdx, sdy);
			DebugOut(L"rdx: %f - rdy: %f\n", rdx, rdy);
		}
	}*/
	
		

	CollisionEvent* e = new CollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void GameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<eType> excludeType,
	eType excludeObject)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (this == coObjects->at(i))
			continue;

		if (excludeObject != eType::NONE)
		{
			if (excludeObject != coObjects->at(i)->GetType())
				continue;
		}
		else
		{
			if (excludeType.size() != 0)
			{
				for (eType type : excludeType)
				{
					if (type == coObjects->at(i)->GetType())
						goto continueLoop;
				}
			}

			if (false)
			{
			continueLoop:
				continue;
			}
		}

		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);		
			
		}
		else
			delete e;

	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
}

void GameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->nx != 0) {
			if (c->obj->GetType() == eType::PLATFORM)
			{
				min_tx = c->t; nx = 0; min_ix = i; rdx = c->dx;
			}
			else if (c->obj->GetType() == eType::ITEM)
			{
				min_tx = nx = rdx = 0; min_ix = i;
			}
			else
			{
				min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
			}
			coEventsResult.push_back(coEvents[min_ix]);
		}

		if (c->ny != 0) {
			if (c->obj->GetType() == eType::PLATFORM)
			{
				min_ty = c->t; min_iy = i; rdy = c->dy;

				if (c->ny > 0)
					ny = 0;
				else
					ny = c->ny;
			}
			else if (c->obj->GetType() == eType::ITEM)
			{
				min_ty = ny = rdy = 0; min_iy = i;
			}
			else if (c->obj->GetType() == eType::PLAYER && this->type != eType::GROUP_MOVING)
			{
				min_ty = ny = rdy = 0; min_iy = i;
			}
			else
			{
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
			coEventsResult.push_back(coEvents[min_iy]);
		}
	}

	sort(coEventsResult.begin(), coEventsResult.end(),
		[](const LPCOLLISIONEVENT& lhs, const LPCOLLISIONEVENT& rhs)
		{
			return lhs->obj->GetDrawOrder() > rhs->obj->GetDrawOrder();
		}
	);

	//if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	//if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);

	/*
	if (this->type == eType::ENEMY)
	{
		DebugOut(L"size: %d\n", coEvents.size());

	}
	*/
}


void GameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = Texture::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	//DebugOut(L"right: %d -- bottom: %d\n", rect.right, rect.bottom);

	GameEngine::GetInstance()->Draw(l, t, bbox, rect.left, rect.top, rect.right, rect.bottom, D3DCOLOR_ARGB(125,255,255,255));
}



GameObject::~GameObject()
{

}