#pragma once
#include "Object.h"


class Enemy : public Object
{
public:
	Enemy();
	Enemy(Enemy& other);
	~Enemy();

	/* --------- 가상함수 --------- */
	virtual bool	Init();
	virtual int		Update(float deltaTime);
	virtual bool	Render(HDC hdc);
	virtual bool	End();

private:
	/* --------- 사용할 함수 --------- */
	void			InitDestPos();
	void			InitBaseDestPos();
	void			InitLevel();

	void			PlayerHit(shared_ptr<Collider> src, shared_ptr<Collider> dest, float deltaTime);
	
	/* ------- 5. 적은 현재 위치에서 랜덤 방향으로 2초에 한번씩 이동 ---------*/
	void			EnumyMove(float deltaTime);

private:
	/* --------- 이름 관련 --------- */	
	static int		count;

private:
	/* ---------- 움직임 ----------- */
	Vec3			_destination;
	Vec3			_destDir;

	float			_coolTime = 0.0f;
	float			_coolMaxTime = 2.0f;

	float			_speed = 70.0f;
	int				_level = 1;
};

