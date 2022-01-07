#pragma once

#include "Collider.h"

/*-----------------------
*	RectCollider
-----------------------*/
class RectCollider : public Collider
{
public:
	RectCollider();
	RectCollider(RectCollider& other);
	virtual ~RectCollider();

public:
	/* ---------- 가상 함수 --------------- */
	virtual bool	Init();
	virtual int		Update();
	virtual bool	Render(HDC hdc);
	virtual bool	End();

	virtual bool	Collision(shared_ptr<Collider> dest);

public:
	/* ---------- 헬퍼 함수 --------------- */
	void	SetWidth(int32 width);
	void	SetHeight(int32 height);

	int32	GetWidth();
	int32	GetHeight();

private:
	int32	_width = 30;
	int32	_height = 30;
};

