#include "framework.h"
#include "RectCollider.h"
#include "Object.h"
#include "Shape.h"


RectCollider::RectCollider()
{
	_width = 30;
	_height = 30;
}

RectCollider::RectCollider(RectCollider& other)
{
	_width = other._width;
	_height = other._height;
}

RectCollider::~RectCollider()
{
}

bool RectCollider::Init()
{
	Collider::Init();

	Vec3 pos = _obj->GetPos();
	Object::WinToDescart(pos);

	_boundary.x = pos.x + _pos.x;
	_boundary.y = pos.y + _pos.y;
	_boundary.w = _width;
	_boundary.h = _height;

	_type = COLLIDER_TYPE::CT_RECT;

	return true;
}

int RectCollider::Update()
{
	Collider::Update();

	// 객체는 계속 움직이므로 수치를 조정해준다.
	Vec3 pos = _obj->GetPos();
	Object::WinToDescart(pos);

	_boundary.x = pos.x + _pos.x;
	_boundary.y = pos.y + _pos.y;
	_boundary.w = _width;
	_boundary.h = _height;

	return 0;
}

bool RectCollider::Render(HDC hdc)
{
	Collider::Render(hdc);

#pragma region 디버깅용 Rectangle Draw
	Vec3 pos = _obj->GetPos();
	Object::WinToDescart(pos);

	int32 x = _boundary.x;
	int32 y = _boundary.y;

	for (int32 i = -_width; i < _width; ++i) {
		SetPixel(hdc, x + i, y - _height, _color.value);
	}

	for (int32 i = -_height; i < _height; ++i) {
		SetPixel(hdc, x + _width, y + i, _color.value);
		SetPixel(hdc, x - _width, y + i, _color.value);
	}

	for (int32 i = -_width; i < _width; ++i) {
		SetPixel(hdc, x + i, y + _height, _color.value);
	}
#pragma endregion
	
	return true;
}

bool RectCollider::End()
{
	Collider::End();

	return true;
}

bool RectCollider::Collision(shared_ptr<Collider> dest)
{
	switch (dest->GetType()) {
	case COLLIDER_TYPE::CT_RECT:
		return Math::CollisionAABB(_boundary, dest->GetBoundary());
		break;
	}

	return false;
}


void RectCollider::SetWidth(int32 width)
{
	_width = width;
}

void RectCollider::SetHeight(int32 height)
{
	_height = height;
}

int32 RectCollider::GetWidth()
{
	return _width;
}

int32 RectCollider::GetHeight()
{
	return _height;
}
