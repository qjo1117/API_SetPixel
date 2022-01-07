#pragma once

#include "Object.h"

// [0] ������ / [1] ������
class Line : public Object
{
public:
	/* --------- ������ --------- */
	Line();
	Line(Line& other);
public:
	/* --------- �����Լ� --------- */
	virtual bool	Init();
	virtual int		Update(float deltaTime);
	virtual bool	Render(HDC hdc);
	virtual bool	End();

public:
	void SetStart(Vec3 start);
	void SetStart(float x, float y);
	void SetEnd(Vec3 end);
	void SetEnd(float x, float y);

	Vec3& GetStart();
	Vec3& GetEnd();

private:
	void InitVertex();

private:
	Vec3 _start, _end;
};

class Circle : public Object
{
public:
	/* --------- ������ --------- */
	Circle();
	Circle(Circle& other);
public:
	/* --------- �����Լ� --------- */
	virtual bool	Init();
	virtual int		Update(float deltaTime);
	virtual bool	Render(HDC hdc);
	virtual bool	End();

public:
	/* --------- ���� �Լ� --------- */
	void SetRadius(float radius);

	float GetRadius(void);

private:
	void InitVertex();

private:
	float _radius;

};



/* -----------
	Rect
  ----------- */

class Rect : public Object
{
public:
	Rect();
	Rect(Rect& rect);

public:
	virtual bool	Init();
	virtual int		Update(float deltaTime);
	virtual bool	Render(HDC hdc);
	virtual bool	End();

public:
	void SetRect(float width, float height);
	void SetWidth(float width);
	void SetHeight(float height);

	const float GetWidth(void) const;
	const float GetHeight(void) const;

private:
	void RednerRect(HDC hdc);
	void InitVertex();

	void Case1(HDC hdc);		// �ϵ� �ڵ�
	void Case2(HDC hdc);		// t1 * r * t2
	void Case3(HDC hdc);		// t2 * r * t1

protected:
	float _width, _height;
};

