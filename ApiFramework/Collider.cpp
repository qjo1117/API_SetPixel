#include "framework.h"
#include "Collider.h"
#include "Object.h"
#include "Shape.h"

Collider::Collider()
{
}

Collider::Collider(Collider& other)
{
}

Collider::~Collider()
{
}

bool Collider::Init()
{
	return true;
}

int Collider::Update()
{
	return 0;
}

bool Collider::Render(HDC hdc)
{
	return true;
}

bool Collider::End()
{
	return true;
}

bool Collider::Collision(shared_ptr<Collider> dest)
{
	return false;
}


void Collider::SetPos(Vec3 pos)
{
	_pos = pos;
}

void Collider::SetBoundary(Quad boundary)
{
	_boundary = boundary;
}

void Collider::SetObj(shared_ptr<Object> obj)
{
	_obj = obj;
}

void Collider::SetColor(Color color)
{
	_color = color;
}

Vec3& Collider::GetPos()
{
	return _pos;
}

Quad& Collider::GetBoundary()
{
	return _boundary;
}

shared_ptr<Object> Collider::GetObj()
{
	return _obj;
}

COLLIDER_TYPE Collider::GetType()
{
	return _type;
}

