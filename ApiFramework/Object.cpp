#include "framework.h"
#include "Object.h"
#include "Core.h"
#include "Collider.h"

/* -----------
	Object
  ----------- */

Object::Object() : 
	_tag(L"Known"), 
	_pos(0.0f, 0.0f, 0.0f), 
	_color(),
	_enable(true),
	_parent(nullptr),
	_angle(0.0f),
	_scale(1.0f),
	_parentAngle(0.0f),
	_parentScale(1.0f)
{
}

Object::Object(Object& other) :
	_tag(other._tag),
	_pos(other._pos),
	_color(other._color),
	_enable(other._enable),
	_parent(other._parent),
	_angle(other._angle),
	_scale(other._scale),
	_parentAngle(other._angle),
	_parentScale(other._scale)
{
	for (Vec3& vertex : other._vertexs) {
		_vertexs.push_back(vertex);
	}

	for (shared_ptr<Object>& child : other._childs) {
		_childs.push_back(child);
	}
}

Object::~Object()
{
	End();
}

bool Object::Init()
{

	return true;
}

int Object::Update(float deltaTime)
{


	return 0;
}

bool Object::Render(HDC hdc)
{
	if (_parent != nullptr) {
		_parentAngle = _parent->_angle;
		_parentScale = _parent->_scale;
		_angle = _parent->_angle;
		_scale = _parent->_scale;

		Matrix mat = IdentityMatrix();

		mat = MatrixSRT(_parentScale, _parentAngle);

		Vec4 tempPos = _baseOffset;
		_offset = MultipleMatrixToVector4(mat, tempPos);
		_pos = _offset + _parent->GetPos();
	}

	return true;
}

bool Object::End()
{
	for (auto& child : _childs) {
		child = nullptr;
	}
	_parent = nullptr;

	
	if (_collider != nullptr) {
		_collider->GetObj().reset();
		_collider = nullptr;
	}

	return true;
}

void Object::DescartToWin(Vec3& vertex)
{
	vertex.x -= CORE->GetRect().right / 2;
	vertex.y = -vertex.y + CORE->GetRect().bottom / 2;
}

void Object::DescartToWin(vector<Vec3>& vertexs)
{
	for (Vec3& vertex : vertexs) {
		Object::DescartToWin(vertex);
	}
}

void Object::WinToDescart(Vec3& vertex)
{
	vertex.x += CORE->GetRect().right / 2;
	vertex.y = -vertex.y + CORE->GetRect().bottom / 2;
}

void Object::WinToDescart(vector<Vec3>& vertexs)
{
	for (Vec3& vertex : vertexs) {
		Object::WinToDescart(vertex);
	}
}

void Object::Attach(shared_ptr<Object> obj, Vec3 offset)
{
	obj->_parent = shared_from_this();

	Matrix mat = IdentityMatrix();
	RotationMatrixX(mat, _angle);
	Vec4 tempPos1 = (obj->GetPos() - _pos) + offset;
	obj->_baseOffset = MultipleMatrixToVector4(mat, tempPos1);
	obj->_offset = _baseOffset;
	obj->_pos = _offset;

	obj->_angle = _angle;
	_childs.push_back(obj);
}

shared_ptr<Object> Object::DisAttach(const wstring& tag)
{
	vector<shared_ptr<Object>>::iterator iterEnd = _childs.end();
	vector<shared_ptr<Object>>::iterator iter = _childs.begin();
	for (; iterEnd != iter;) {
		if (iter->get()->GetTag() == tag) {
			shared_ptr<Object> obj = *iter;
			obj->_parent = nullptr;
			iter = _childs.erase(iter);
			return obj;
		}
		++iter;
	}

	return nullptr;
}

shared_ptr<Object> Object::FindChild(const wstring& tag)
{
	for (auto& child : _childs) {
		if (child->_tag == tag) { 
			return child;
		}
		if (child->_childs.size() >= 0) {
			shared_ptr<Object> findObj = child->FindChild(tag);
			if (findObj != nullptr) {
				return findObj;
			}
		}
	}

	return nullptr;
}

void Object::SetParentPosition()
{
	Vec3 temp = (_parent->GetPos() - _pos) + _parent->_offset;

	Matrix mat = IdentityMatrix();
	RotationMatrixX(mat, _angle);
	Vec4 tempPos1 = (_parent->GetPos() - _pos) + _parent->_offset;
	_parent->_offset = MultipleMatrixToVector4(mat, tempPos1);

	_parent->_angle = _angle;
}

void DrawLine(HDC hdc, Vec3 pos, Vec3& dest, Vec3& src, COLORREF color)
{
	Vec3 nor = (src - dest);
	float distance = nor.Normalize();
	nor = nor.NormalVector3();
	pos = pos + dest;
	for (float i = 0; i < distance; ++i) {
		SetPixel(hdc, static_cast<int>(pos.x), static_cast<int>(pos.y), color);
		pos = pos + nor;
	}
}