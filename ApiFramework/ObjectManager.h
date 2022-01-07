#pragma once

#include "Object.h"

class Object;


class ObjectManager
{
public:
	friend class Object;
private:
	ObjectManager();
	~ObjectManager();
public:
	/* -------- �̱��� -------- */
	static ObjectManager* GetInstance() {
		static ObjectManager instance;
		return &instance;
	}

public:
	/* -------- �⺻ �Լ� -------- */
	int Init();
	int Update(float deltaTime);
	int Render(HDC hdc);
	int End();

public:
	template<typename T>
	shared_ptr<T> CreateObject() {
		shared_ptr<T> obj = make_shared<T>();
		_listGameObject.push_back(obj);
		obj->Init();
		return obj;
	}

	shared_ptr<Object> FindObject(const wstring& tag) {
		for (auto& obj : _listGameObject) {
			if (obj->GetTag() == tag) {
				return obj;
			}
		}
		return nullptr;
	}

	// ���� �Ⱦ��� ����� shared_ptr�̶�
	// ���� ������ ���ص� ������ ��
	void DeleteObject(const wstring& tag) {

		return;
	}



private:
	static list<shared_ptr<Object>> _listGameObject;
};

