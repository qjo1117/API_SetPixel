#include "framework.h"
#include "ObjectManager.h"



list<shared_ptr<Object>> ObjectManager::_listGameObject;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	for (auto& obj : _listGameObject) {
		for (int32 i = 0; i < obj.use_count(); ++i) {
			obj.reset();
		}
	}
}

int ObjectManager::Init()
{
	for (auto& obj : _listGameObject) {
		if (obj->GetActive()) {
			obj->Init();
		}
	}
	return 0;
}

int ObjectManager::Update(float deltaTime)
{
	for (auto& obj : _listGameObject) {
		if (obj->GetActive()) {
			obj->Update(deltaTime);
		}
	}

	return 0;
}

int ObjectManager::Render(HDC hdc)
{
	for (auto& obj : _listGameObject) {
		if (obj->GetActive() == true) {
			obj->Render(hdc);
		}
	}
	return 0;
}

int ObjectManager::End()
{
	for (auto& obj : _listGameObject) {
		obj->End();
	}

	return 0;
}
