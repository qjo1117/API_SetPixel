#pragma

class Collider;

class QuadTree
{
public:
	QuadTree();
	~QuadTree();

public:
	void Insert(shared_ptr<Collider> collider);
	void QuarryRange(Quad& range, set<shared_ptr<Collider>>& vec);
	void SetParent(shared_ptr<QuadTree> parent);

	void Init(int32 width, int32 height, int32 capacity = 4);
	void Init(Quad rect, int32 capacity = 4);
	void Render(HDC hdc);
	void Clear();

private:
	void SubDivid();

private:
	shared_ptr<QuadTree>			_rightUp;
	shared_ptr<QuadTree>			_leftUp;
	shared_ptr<QuadTree>			_rightDown;
	shared_ptr<QuadTree>			_leftDown;

	shared_ptr<QuadTree>			_parent;

	Quad							_boundary = { 0,0,0,0 };

	vector<shared_ptr<Collider>>	_vecList;

	int32							_capacity = 4;
	int32							_count = 0;


	bool							_divided = false;
	static int32					TreeCount;
};


bool CollisionRectToPoint(int32 x, int32 y, int32 width, int32 height, Vec2& pos);