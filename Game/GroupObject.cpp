#include "GroupObject.h"
#include "debug.h"

GroupObject::GroupObject()
{
	left = top = 70000;
	right = bottom = -1;
	vx = vy = 0;

}

/*
	Thêm object vào group
*/
void GroupObject::Add(LPGAMEOBJECT& gameObject)
{
	group.push_back(gameObject);

	//Tính lại tổng thể bounding box của group
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

	//Tính lại width và height
	this->width = int(right - left);
	this->height = int(bottom - top);

}

/*
	Dịch chuyển group một khoảng x, y
*/
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

/*
	Đặt lại vị trí group tại x, y
*/
void GroupObject::SetGroupPos(float x, float y)
{
	float currentOffsetX = 0.0f;
	for (LPGAMEOBJECT object : group)
	{
		object->SetPosition(x + currentOffsetX, y);
		currentOffsetX += STANDARD_SIZE;
	}
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
	//Kiểm tra nếu ra khỏi camera thì dừng
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

	//Nếu group di chuyển được
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

	//RenderBoundingBox();
}

/*
	Xóa group trong Unload
*/
void GroupObject::Destroy_Group()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	//Kiểm tra xem group này có thuộc grid nào khác nữa không, nếu có thì chuyển sang grid đó
	//GroupObject không có currentCell, khi dùng hàm AddToCell nó sẽ tự động gán currentCell cho object nên ta phải đặt lại
	//tránh trường hợp xóa lại nhiều lần
	for (int cell : inCell)
	{
		if (Global::GetInstance()->cells.find(cell) != Global::GetInstance()->cells.end())
		{
			current->AddToCell(cell, this);
			this->currentCell = -1;
			return;
		}
	}

	current->Destroy(this);
}

/*
	Xóa group, giống cái trên
*/
void GroupObject::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	//Kiểm tra xem group này có thuộc grid nào khác nữa không, nếu có thì chuyển sang grid đó
	for (int cell : inCell)
	{
		if (Global::GetInstance()->cells.find(cell) != Global::GetInstance()->cells.end())
		{
			current->AddToCell(cell, this);
			this->currentCell = -1;
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
	//Xóa hết mọi thứ trong group
	int i, j;
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	GLOBAL global = Global::GetInstance();

	for (LPGAMEOBJECT object : group)
	{
		object->GetIndex(i, j);
		global->occupiedGroup.erase(global->TwoDimension_To_OneDimension(i, j, scene->height));
		delete object;
		object = NULL;
	}

	group.clear();
}
