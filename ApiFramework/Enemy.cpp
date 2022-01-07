#include "framework.h"
#include "Enemy.h"
#include "Shape.h"
#include "ObjectManager.h"
#include "Math.h"
#include "Core.h"
#include "RectCollider.h"
#include "ColliderManager.h"
#include "Player.h"

int Enemy::count = 0;

Enemy::Enemy() : Object()
{
}

Enemy::Enemy(Enemy& other) : Object(other)
{
}

Enemy::~Enemy()
{
}

bool Enemy::Init()
{
	Object::Init();

	/* ----- 2. 적 캐릭터 크기랜덤 ----- */
	Random random;
	random.Init(Enemy::count);
	_level = random.GetValue(1, 7);
	InitLevel();

#pragma region 이름 정해주기
	_tag = L"Monster";
	Enemy::count += 1;
	WCHAR str[100] = { 0 };
	wsprintfW(str, L"%s_%d", _tag.data(), Enemy::count);
	_tag = str;
#pragma endregion
	
#pragma region 파츠 생성 및 연결
	{
		shared_ptr<Rect> obj = OBJECT->CreateObject<Rect>();
		obj->SetPos(0.0f, 0.0f, 0.0f);
		obj->SetWidth(50.0f);
		obj->SetHeight(40.0F);
		obj->SetColor(_color);

		this->Attach(obj, Vec3{ 0.0f, 15.0f, 0.0f });
	}

	{
		shared_ptr<Rect> obj = OBJECT->CreateObject<Rect>();
		obj->SetWidth(20.0f);
		obj->SetHeight(30.0f);
		obj->SetColor(_color);

		this->Attach(obj, Vec3{ 30.0f, -60.0f, 0.0f });
	}

	{
		shared_ptr<Rect> obj = OBJECT->CreateObject<Rect>();
		obj->SetWidth(20.0f);
		obj->SetHeight(30.0f);
		obj->SetColor(_color);

		this->Attach(obj, Vec3{ -30.0f, -60.0f, 0.0f });
	}
	

#pragma endregion

	InitBaseDestPos();

	shared_ptr<RectCollider> collider = COLLIDER->CreateCollider<RectCollider>(shared_from_this());
	collider->SetWidth(50.0f * _scale);
	collider->SetHeight(75.0f * _scale);
	collider->SetPos(Vec3{ 0.0f, collider->GetHeight() * 0.2f, 0.0f});
	collider->AddCollisionFunction(static_pointer_cast<Enemy>(shared_from_this()), &Enemy::PlayerHit);


	return true;
}

int Enemy::Update(float deltaTime)
{
	Object::Update(deltaTime);

	EnumyMove(deltaTime);

	return 0;
}

bool Enemy::Render(HDC hdc)
{
	Object::Render(hdc);

	return true;
}

bool Enemy::End()
{
	Object::End();

	return true;
}

void Enemy::InitDestPos()
{
	Random rand;
	int32 width = (int32)CORE->GetRect().right / 2;
	int32 height = (int32)CORE->GetRect().bottom / 2;

	int32 x = static_cast<int32>(rand.GetValue(-width, width));
	int32 y = static_cast<int32>(rand.GetValue(-height, height));

	_destination = Vec3((float)x, (float)y, 0.0f);
}

void Enemy::InitBaseDestPos()
{
	int32 width = static_cast<int32>(CORE->GetRect().right / 2);
	int32 height = static_cast<int32>(CORE->GetRect().bottom / 2);

	int32 x = static_cast<int32>(rand() % (width * 2) - width);
	int32 y = static_cast<int32>(rand() % (height * 2) - height);

	_destination = Vec3((float)x, (float)y, 0.0f);
	_destDir = (_destination - _pos).NormalVector3();
}

void Enemy::InitLevel()
{
	switch (_level) {
	case 1:
		_scale = 0.3f;
		_color = RGB(0, 0, 0);
		_speed = 120.0f;
		break;

	case 2:
		_scale = 0.4f;
		_color = RGB(0, 200, 0);
		_speed = 100.0f;
		break;

	case 3:
		_scale = 0.5f;
		_color = RGB(0, 0, 255);
		_speed = 70.0f;
		break;

	case 4:
		_scale = 0.6f;
		_color = RGB(255, 0, 0);
		_speed = 50.0f;
		break;

	case 5:
		_scale = 0.7f;
		_color = RGB(125, 125, 0);
		_speed = 40.0f;
		break;

	case 6:
		_scale = 1.0f;
		_color = RGB(0, 125, 125);
		_speed = 30.0f;
		break;
	}
}

void Enemy::PlayerHit(shared_ptr<Collider> src, shared_ptr<Collider> dest, float deltaTime)
{
	if (dest->GetObj()->GetTag() != L"Player") {
		return;
	}
	if (src->GetObj()->GetActive() == false) {
		return;
	}

	shared_ptr<Player> player = static_pointer_cast<Player>(dest->GetObj());
	if (player->GetDead() == true) {
		return;
	}

	if (player->GetLevel() >= _level) {
		player->LevelUp();

		SetActive(false);
		player->StartRotate();
	}
	else {
		player->GameOver();
	}

	if (player->GetMonsterCount() > 10) {
		player->GameWin();
	}
}

void Enemy::EnumyMove(float deltaTime)
{
	/* ------- 5. 적은 현재 위치에서 랜덤 방향으로 2초에 한번씩 이동 ---------*/

	if ((_destination - _pos).Length() < 1.0f) {
		_pos = _destination;
		InitBaseDestPos();
	}
	else {
		_destDir = (_destination - _pos).NormalVector3();
		_pos = (_destDir * deltaTime * _speed) + _pos;
	}

	// 2초마다 방향을 바꾸는 부분
	_coolTime += deltaTime;
	if (_coolMaxTime <= _coolTime) {
		InitBaseDestPos();
		_coolTime -= _coolMaxTime;
	}
}
