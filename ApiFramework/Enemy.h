#pragma once
#include "Object.h"


class Enemy : public Object
{
public:
	Enemy();
	Enemy(Enemy& other);
	~Enemy();

	/* --------- �����Լ� --------- */
	virtual bool	Init();
	virtual int		Update(float deltaTime);
	virtual bool	Render(HDC hdc);
	virtual bool	End();

private:
	/* --------- ����� �Լ� --------- */
	void			InitDestPos();
	void			InitBaseDestPos();
	void			InitLevel();

	void			PlayerHit(shared_ptr<Collider> src, shared_ptr<Collider> dest, float deltaTime);
	
	/* ------- 5. ���� ���� ��ġ���� ���� �������� 2�ʿ� �ѹ��� �̵� ---------*/
	void			EnumyMove(float deltaTime);

private:
	/* --------- �̸� ���� --------- */	
	static int		count;

private:
	/* ---------- ������ ----------- */
	Vec3			_destination;
	Vec3			_destDir;

	float			_coolTime = 0.0f;
	float			_coolMaxTime = 2.0f;

	float			_speed = 70.0f;
	int				_level = 1;
};

