#pragma once

#include "Object.h"
#include "QuadTree.h"

class Object;

/*-----------------------
*	ColliderManager
-----------------------*/
class ColliderManager
{
public:
	/* ----------- ΩÃ±€≈Ê ------------ */
	static ColliderManager* GetInstance() {
		static ColliderManager instance;
		return &instance;
	}

	ColliderManager();
	~ColliderManager();

public:
	int		Init(int32 width, int32 height);
	int		Update(float deltaTime);
	int		Render(HDC hdc);
	int		End();

	bool	Collision(float deltaTime);

	template<typename T>
	shared_ptr<T>	CreateCollider(shared_ptr<Object> obj);

private:
	shared_ptr<class QuadTree>			_treeCollider;
	vector<shared_ptr<class Collider>>	_colliders;

	float	_coolTime = 0.0f;
	float	_coolMaxTime = 1.0f;

	bool	_isDebug = true;

	WCHAR	_str[100] = { 0 };
};

template<typename T>
inline shared_ptr<T> ColliderManager::CreateCollider(shared_ptr<Object> obj)
{
	shared_ptr<T> collider = make_shared<T>();

	collider->SetObj(obj);
	obj->SetCollider(collider);
	_treeCollider->Insert(collider);

	_colliders.push_back(collider);

	collider->Init();
	return collider;
}
