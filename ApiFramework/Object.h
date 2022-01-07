#pragma once



/* -----------
	Object
  ----------- */

class Collider;

class Object : public enable_shared_from_this<Object>
{
protected:
	/* --------- 생성자 --------- */
	Object();
	Object(Object& other);
	virtual ~Object();
public:
	/* --------- 가상함수 --------- */
	virtual bool	Init();
	virtual int		Update(float deltaTime);
	virtual bool	Render(HDC hdc);
	virtual bool	End();

	/* --------- 셋팅 함수 --------- */
	void SetTag(const wstring& tag) {
		_tag = tag;
	}
	void SetPos(float x, float y, float z) {
		_pos.x = x;
		_pos.y = y;
		_pos.z = z;
	}
	void SetPos(Vec3 pos) {
		SetPos(pos.x, pos.y, pos.z);
	}
	void SetActive(bool enable) {
		_enable = enable;
		for (auto& child : _childs) {
			child->SetActive(_enable);
		}
	}
	void SetColor(Color color) {
		_color = color;
		for (auto& child : _childs) {
			child->SetColor(color);
		}
	}
	void SetAngle(float angle) {
		_angle = angle;
	}
	void SetScale(float scale) {
		_scale = scale;
	}
	void SetMove(Vec3 move) {
		SetMove(move.x, move.y, move.z);
	}
	void SetMove(float x, float y, float z) {
		_move = Vec3{ x,y,z };
	}
	void SetCollider(shared_ptr<Collider> collider) {
		_collider = collider;
	}


	/* --------- 반환 함수 ---------*/
	const wstring& GetTag() const {
		return _tag;
	}
	Vec3& GetPos() {
		return _pos;
	}
	bool GetActive() {
		return _enable;
	}
	float GetAngle() {
		return _angle;
	}
	float& GetScale() {
		return _scale;
	}
	Vec3& GetMove() {
		return _move;
	}
	shared_ptr<Collider> GetCollider() {
		return _collider;
	}

	shared_ptr<Object> GetParent() { return _parent; }

public:
	static void DescartToWin(Vec3& vertex);
	static void DescartToWin(vector<Vec3>& vertexs);
	static void WinToDescart(Vec3& vertex);
	static void WinToDescart(vector<Vec3>& vertexs);

public:
	/* -------- 부모 관련 함수 ------- */

	// 부모.(자식) 으로 전달받는다.
	void Attach(shared_ptr<Object> obj, Vec3 offset);
	shared_ptr<Object> DisAttach(const wstring& tag);
	shared_ptr<Object> FindChild(const wstring& tag);

	void SetParentPosition();

protected:
	wstring						_tag;

	vector<Vec3>				_vertexs;
	Vec3						_pos;
	float						_scale;
	float						_angle;
	
	Vec3						_move;

	Color						_color;

	bool						_enable;

	Vec3						_offset;
	vector<shared_ptr<Object>>	_childs;
	shared_ptr<Object>			_parent;
	Vec4						_baseOffset;
	float						_parentAngle;
	float						_parentScale;

	shared_ptr<Collider>		_collider;
};

void DrawLine(HDC hdc, Vec3 pos, Vec3& dest, Vec3& src, COLORREF color = RGB(0, 0, 0));

