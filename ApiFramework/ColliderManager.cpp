#include "framework.h"
#include "ColliderManager.h"
#include "Collider.h"
#include "Core.h"

ColliderManager::ColliderManager()
{
	wsprintfW(_str, L"Debug Mode On F1 / F2 %d", _isDebug);

}

ColliderManager::~ColliderManager()
{

}

int ColliderManager::Init(int32 width, int32 height)
{
	_treeCollider = make_shared<QuadTree>();
	_treeCollider->Init(CORE->GetRect().right, CORE->GetRect().bottom);

	for (auto& collider : _colliders) {
		collider->Init();
	}

	return 0;
}

int ColliderManager::Update(float deltaTime)
{

	if (::GetAsyncKeyState(VK_F1) & 0x8000) {
		_isDebug = true;
	}
	if (::GetAsyncKeyState(VK_F2) & 0x8000) {
		_isDebug = false;
	}

	for (auto& collider : _colliders) {
		if (collider->GetObj()->GetActive()) {
			collider->Update();
		}
	}

	for (auto& collider : _colliders) {
		if (collider->GetObj()->GetActive()) {
			_treeCollider->Insert(collider);
		}
	}

	Collision(deltaTime);

	return 0;
}

int ColliderManager::Render(HDC hdc)
{
	_treeCollider->Render(hdc);

	TextOutW(hdc, 0, 0, _str, lstrlenW(_str));

	if (_isDebug == false) {
		_treeCollider->Clear();
		return 0;
	}
	for (auto& collider : _colliders) {
		if (collider->GetObj()->GetActive()) {
			collider->Render(hdc);
		}
	}

	_treeCollider->Clear();

	return 0;
}

int ColliderManager::End()
{
	for (auto& collider : _colliders) {
		collider->GetObj().reset();
		collider->End();
	}

	_treeCollider = nullptr;

	return 0;
}

bool ColliderManager::Collision(float deltaTime)
{
	set<shared_ptr<Collider>> vecCheck;

	for (auto& collider : _colliders) {
		_treeCollider->QuarryRange(collider->GetBoundary(), vecCheck);

		for (auto& dest : vecCheck) {
			if (collider->Collision(dest) == true) {
				collider->CallFunction(dest, deltaTime);
			}

		}
	}

	return true;
}
