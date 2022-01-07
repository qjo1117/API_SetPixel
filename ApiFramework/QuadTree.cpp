#include "framework.h"
#include "QuadTree.h"
#include "Collider.h"
#include "Object.h"

int32 QuadTree::TreeCount = 0;

QuadTree::QuadTree()
{

}

QuadTree::~QuadTree()
{
	_vecList.clear();
}

void QuadTree::Insert(shared_ptr<Collider> collider)
{
	Vec2 pos = Vec2{ (float)collider->GetBoundary().x, (float)collider->GetBoundary().y };
	if (!_boundary.CollisionQRectToPoint(pos)) {
		return;
	}

	// 값을 참조하기 위해 포인터 형식으로 Push_back진행
	if (_capacity >= _count) {
		_vecList.push_back(collider);
		_count += 1;
		QuadTree::TreeCount += 1;
	}
	else {
		if (_divided == false) {
			SubDivid();
			_divided = true;
		}

		_leftUp->Insert(collider);
		_rightUp->Insert(collider);
		_leftDown->Insert(collider);
		_rightDown->Insert(collider);

	}
}

void QuadTree::QuarryRange(Quad& range, set<shared_ptr<Collider>>& vec)
{
	if (!Math::CollisionAABB(_boundary, range)) {
		return;
	}
	else {
		for (auto& obj : _vecList) {
			if (Math::CollisionAABB(range, obj->GetBoundary())) {
				vec.insert(obj);
			}

			if (_divided) {
				_leftUp->QuarryRange(range, vec);
				_rightUp->QuarryRange(range, vec);
				_leftDown->QuarryRange(range, vec);
				_rightDown->QuarryRange(range, vec);
			}
		}
	}
}

void QuadTree::SetParent(shared_ptr<QuadTree> parent)
{
	_parent = parent;
}

void QuadTree::Init(int32 width, int32 height, int32 capacity)
{
	_boundary = { 0, 0, width, height };
	_capacity = capacity;
	_divided = false;
	_vecList.clear();
	_count = 0;
}

void QuadTree::Init(Quad rect, int32 capacity)
{
	_boundary = rect;
	_capacity = capacity;
	_divided = false;
	_vecList.clear();
	_count = 0;
}

void QuadTree::Render(HDC hdc)
{
	//Rectangle(hdc, _boundary.x - (_boundary.w / 2) + 3, _boundary.y - (_boundary.h / 2) + 3,
	//	_boundary.x + (_boundary.w / 2) -3, _boundary.y + (_boundary.h / 2) - 3);
	//if (_leftUp != nullptr) {
	//	_leftUp->Render(hdc);
	//}
	//if (_rightUp != nullptr) {
	//	_rightUp->Render(hdc);
	//}
	//if (_leftDown != nullptr) {
	//	_leftDown->Render(hdc);
	//}
	//if (_rightDown != nullptr) {
	//	_rightDown->Render(hdc);
	//}
}

void QuadTree::Clear()
{
	for (auto& obj : _vecList) {
		obj.reset();
	}
	_vecList.clear();

	if (_leftUp != nullptr) {
		_leftUp->Clear();
		_leftUp.reset();
	}
	if (_rightUp != nullptr) {
		_rightUp->Clear();
		_rightUp.reset();
	}
	if (_leftDown != nullptr) {
		_leftDown->Clear();
		_leftDown.reset();
	}
	if (_rightDown != nullptr) {
		_rightDown->Clear();
		_rightDown.reset();
	}
	Init(_boundary, _capacity);
}

void QuadTree::SubDivid()
{
	Quad rect;
	rect = { _boundary.x - (_boundary.w / 2), _boundary.y + (_boundary.h / 2),_boundary.w / 2,_boundary.h / 2 };
	_leftUp = make_shared<QuadTree>();
	_leftUp->Init(rect, _capacity);
	rect = { _boundary.x + (_boundary.w / 2), _boundary.y + (_boundary.h / 2),_boundary.w / 2,_boundary.h / 2 };
	_rightUp = make_shared<QuadTree>();
	_rightUp->Init(rect, _capacity);
	rect = { _boundary.x - (_boundary.w / 2), _boundary.y - (_boundary.h / 2),_boundary.w / 2,_boundary.h / 2 };
	_leftDown = make_shared<QuadTree>();
	_leftDown->Init(rect, _capacity);
	rect = { _boundary.x + (_boundary.w / 2), _boundary.y - (_boundary.h / 2),_boundary.w / 2,_boundary.h / 2 };
	_rightDown = make_shared<QuadTree>();
	_rightDown->Init(rect, _capacity);
}

bool CollisionRectToPoint(int32 x, int32 y, int32 width, int32 height, Vec2& pos)
{
	if (x - width <= pos.x && pos.x <= x + width) {
		if (y - height <= pos.y && pos.y <= y + height) {
			return true;
		}
	}
	return false;
}

