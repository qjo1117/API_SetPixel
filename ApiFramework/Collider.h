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
	/* ---------- 가상 함수 --------------- */
	virtual bool	Init();
	virtual int		Update();
	virtual bool	Render(HDC hdc);
	virtual bool	End();

	// 오버라이딩으로 체크해서 충돌여부를 반환한다.
	virtual bool	Collision(shared_ptr<Collider> dest);

	// 함수 등록
	template<typename T>
	void AddCollisionFunction(shared_ptr<T> obj,
		void (T::* pFunc)(shared_ptr<Collider>, shared_ptr<Collider>, float)) {
		function<void(shared_ptr<Collider>, shared_ptr<Collider>, float)> func;
		func = bind(pFunc, obj, placeholders::_1, placeholders::_2, placeholders::_3);
		_listFunc.push_back(func);
	}

	// ColliderManager에서만 사용하는 호출 함수
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
	// 콜라이더의 상대적인 좌표
	Vec3				_pos = { 0.0f,0.0f,0.0f };
	Quad				_boundary;
	Color				_color = RGB(255, 0, 0);

	// 자신과 연결되어 있는지 확인하는 오브젝트
	shared_ptr<Object>	_obj;
	COLLIDER_TYPE		_type;

	// 호출할 함수
	list<function<void(shared_ptr<Collider>, shared_ptr<Collider>, float)>> _listFunc;
};

