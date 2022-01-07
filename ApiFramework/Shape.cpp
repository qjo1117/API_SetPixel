#include "framework.h"
#include "Shape.h"


Line::Line() :
	_start(0.0f, 0.0f, 0.0f),
	_end(5.0f, 5.0f, 5.0f)
{
	Init();
}

Line::Line(Line& other) :
	Object(other),
	_start(other._start),
	_end(other._end)
{
}

bool Line::Init()
{
	_vertexs.resize(2);
	InitVertex();

	return true;
}

int Line::Update(float deltaTime)
{

	Object::Update(deltaTime);

	return 0;
}

bool Line::Render(HDC hdc)
{
	Object::Render(hdc);

	Matrix mat = IdentityMatrix();

	mat = MatrixSRT(_scale, _angle);

	Vec4 tempPos1 = _vertexs[0];
	Vec4 vertex1 = MultipleMatrixToVector4(mat, tempPos1);

	// ----------------------------------------------------

	mat = MatrixSRT(_scale, -_angle);
	Vec4 tempPos2 = _vertexs[1];
	Vec4 vertex2 = MultipleMatrixToVector4(mat, tempPos2);

	Object::WinToDescart(_pos);
	DrawLine(hdc, _pos, vertex1, vertex2, _color.value);
	Object::DescartToWin(_pos);

	return true;
}

bool Line::End()
{
	Object::End();
	return true;
}

void Line::SetStart(Vec3 start)
{
	SetStart(start.x, start.y);
}

void Line::SetStart(float x, float y)
{
	_start.x = x;
	_start.y = -y;
	InitVertex();
}

void Line::SetEnd(Vec3 end)
{
	SetEnd(end.x, end.y);
}

void Line::SetEnd(float x, float y)
{
	_end.x = x;
	_end.y = -y;
	InitVertex();
}

Vec3& Line::GetStart()
{
	return _start;
}

Vec3& Line::GetEnd()
{
	return _end;
}

void Line::InitVertex()
{
	_vertexs[0] = _start;
	_vertexs[1] = _end;
}

Circle::Circle() :
	_radius(30.0f)
{
	Init();
}

Circle::Circle(Circle& other) :
	Object(other),
	_radius(other._radius)
{
}

bool Circle::Init()
{
	_vertexs.reserve(1000);
	InitVertex();

	return true;
}

int Circle::Update(float deltaTime)
{
	Object::Update(deltaTime);

	return 0;
}

bool Circle::Render(HDC hdc)
{
	Object::Render(hdc);

	Matrix mat = MatrixSRT(_parentScale, 0.0f);

	mat = MatrixSRT(_scale, _angle);
	Object::WinToDescart(_pos);
	for (auto& vertex : _vertexs) {
		Vec4 tempPos = vertex;
		tempPos = MultipleMatrixToVector4(mat, tempPos);
		SetPixel(hdc, static_cast<int>(_pos.x + tempPos.x), static_cast<int>(_pos.y + tempPos.y), _color.value);
	}
	Object::DescartToWin(_pos);

	return false;
}

bool Circle::End()
{
	Object::End();
	return false;
}

void Circle::SetRadius(float radius)
{
	_radius = radius;
	InitVertex();
}

float Circle::GetRadius(void)
{
	return _radius;
}

void Circle::InitVertex()
{
	_vertexs.clear();

	for (float i = 0; i < _radius * 3.14f; i += 0.5f) {
		_vertexs.push_back(Vec3{ _radius * cosf(i), _radius * sinf(i), 0.0f });
	}
}

Rect::Rect() :
	_width(50.0f),
	_height(50.0f)
{
	Init();
}

Rect::Rect(Rect& rect) :
	Object(rect),
	_width(rect._width),
	_height(rect._height)
{
}

bool Rect::Init()
{
	_vertexs.resize(4);

	InitVertex();

	_pos = Vec3{ 0.0f,0.0f,0.0f };

	return true;
}

int Rect::Update(float deltaTime)
{
	Object::Update(deltaTime);



	return 0;
}

bool Rect::Render(HDC hdc)
{
	Object::Render(hdc);

	RednerRect(hdc);


	return true;
}

bool Rect::End()
{
	Object::End();
	return false;
}

void Rect::SetRect(float width, float height)
{
	SetWidth(width);
	SetHeight(height);
}

void Rect::SetWidth(float width)
{
	_width = width;
	InitVertex();
}

void Rect::SetHeight(float height)
{
	_height = height;
	InitVertex();
}

const float Rect::GetWidth(void) const
{
	return _width;
}

const float Rect::GetHeight(void) const
{
	return _height;
}

void Rect::RednerRect(HDC hdc)
{
	Matrix mat = IdentityMatrix();
	mat = MatrixSRT(_scale, -_angle);

	if (_parent != nullptr) {
		mat = MatrixSRT(_parentScale, 0.0f);

	}

	mat = MatrixSRT(_scale, -_angle);
	Object::WinToDescart(_pos);
	int size = _vertexs.size();
	for (int32 i = 0; i < size; ++i) {
		Vec4 tempPos1 = _vertexs[i];
		Vec4 vertex1 = MultipleMatrixToVector4(mat, tempPos1);

		Vec4 tempPos2 = _vertexs[(i + 1) % size];
		Vec4 vertex2 = MultipleMatrixToVector4(mat, tempPos2);

		DrawLine(hdc, _pos, vertex1, vertex2, _color.value);
	}
	Object::DescartToWin(_pos);
}

void Rect::InitVertex()
{
	_vertexs[0].x = -_width;
	_vertexs[0].y = +_height;
	_vertexs[0].z = 0.0f;

	_vertexs[1].x = +_width;
	_vertexs[1].y = +_height;
	_vertexs[1].z = 0.0f;

	_vertexs[2].x = +_width;
	_vertexs[2].y = -_height;
	_vertexs[2].z = 0.0f;

	_vertexs[3].x = -_width;
	_vertexs[3].y = -_height;
	_vertexs[3].z = 0.0f;
}
