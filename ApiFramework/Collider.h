#pragma once


class Object;
class Rect;

/*-----------------------
*		Collider
-----------------------*/
class Collider : public enable_shared_from_this<Collider>
{
public:
	Collider();
	Collider(Collider& other);
	~Collider();

public:
	/* ---------- ���� �Լ� --------------- */
	virtual bool	Init();
	virtual int		Update();
	virtual bool	Render(HDC hdc);
	virtual bool	End();

	// �������̵����� üũ�ؼ� �浹���θ� ��ȯ�Ѵ�.
	virtual bool	Collision(shared_ptr<Collider> dest);

	// �Լ� ���
	template<typename T>
	void AddCollisionFunction(shared_ptr<T> obj,
		void (T::* pFunc)(shared_ptr<Collider>, shared_ptr<Collider>, float)) {
		function<void(shared_ptr<Collider>, shared_ptr<Collider>, float)> func;
		func = bind(pFunc, obj, placeholders::_1, placeholders::_2, placeholders::_3);
		_listFunc.push_back(func);
	}

	// ColliderManager������ ����ϴ� ȣ�� �Լ�
	void CallFunction(shared_ptr<Collider> dest, float deltaTime) {
		for (auto& func : _listFunc) {
			func(shared_from_this(), dest, deltaTime);
		}
	}

public:
	void				SetPos(Vec3 pos);
	void				SetBoundary(Quad boundary);
	void				SetObj(shared_ptr<Object> obj);
	void				SetColor(Color color);

	Vec3&				GetPos();
	Quad&				GetBoundary();
	shared_ptr<Object>	GetObj();
	COLLIDER_TYPE		GetType();

protected:
	// �ݶ��̴��� ������� ��ǥ
	Vec3				_pos = { 0.0f,0.0f,0.0f };
	Quad				_boundary;
	Color				_color = RGB(255, 0, 0);

	// �ڽŰ� ����Ǿ� �ִ��� Ȯ���ϴ� ������Ʈ
	shared_ptr<Object>	_obj;
	COLLIDER_TYPE		_type;

	// ȣ���� �Լ�
	list<function<void(shared_ptr<Collider>, shared_ptr<Collider>, float)>> _listFunc;
};

