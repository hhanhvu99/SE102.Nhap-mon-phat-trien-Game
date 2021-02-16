#include "Rubbish.h"

Rubbish::Rubbish(float x, float y, float speedX, float speedY, float deflect, int direction)
{
	this->x = x;
	this->y = y;
	this->speedX = speedX;
	this->speedY = speedY;
	this->direction = direction;
	this->currentDergee = 0.0f;
	this->animation_set = AnimationManager::GetInstance()->Get(BULLET);
	this->vy = -deflect;

	this->Add();
}

/*
	Thêm vào scene dưới dạng Background
*/
void Rubbish::Add()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);
	current->Add_Visual(this);
}

/*
	Xóa khỏi scene
*/
void Rubbish::Destroy()
{
	LPSCENE scene = SceneManager::GetInstance()->GetCurrentScene();
	LPTESTSCENE current = static_cast<LPTESTSCENE>(scene);

	current->Destroy_Visual(this);
}

void Rubbish::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void Rubbish::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	x += dx;
	y += dy;

	//Nếu góc hiện tại lớn hơn 360, reset lại góc
	if (currentDergee >= 360.0f)
		currentDergee = 0.0f;
	//Xoay gạch với 1 góc cố định
	else
		currentDergee += RUBBISH_ROTATE_SPEED * dt;

	//Tốc độ vx, vy để tạo hiệu ứng vòng
	vx = direction * speedX * dt;
	vy += speedY * dt;

	//Ra khỏi scene, xóa
	GameEngine::GetInstance()->GetCamPos(camPosX, camPosY);
	if (x < camPosX - BULLET_SAFE_DELETE_RANGE || x > camPosX + SCREEN_WIDTH + BULLET_SAFE_DELETE_RANGE ||
		y < camPosY - BULLET_SAFE_DELETE_RANGE || y > camPosY + SCREEN_HEIGHT + BULLET_SAFE_DELETE_RANGE)
	{
		this->Destroy();
		return;
	}
}

void Rubbish::Render()
{
	//Ani thuộc phần BULLET
	int ani = -1;

	if (direction > 0) ani = BULLET_EFFECT_RUBBISH + BULLET_ANI_RIGHT;
	else ani = BULLET_EFFECT_RUBBISH + BULLET_ANI_LEFT;

	animation_set->Get(ani)->Render(x, y, currentDergee);
}
