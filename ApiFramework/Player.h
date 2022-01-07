#pragma once

#include "Object.h"

/* -----------
	Player
  ----------- */


class Player : public Object
{
public:
	/* --------- 생성자 --------- */
	Player();
	Player(Player& other);
public:
	/* --------- 가상함수 --------- */
	virtual bool	Init();
	virtual int		Update(float deltaTime);
	virtual bool	Render(HDC hdc);
	virtual bool	End();

private:
	void RotationObject(const wstring& name,float angle);

private:
	bool Assembly();

private:
	void CreatePart();

	/* ------- 4. WSAD를 이용해서 플레이어가 8방량으로 움직임 ---------*/
	void InputMove(float deltaTime);
	void CheckDead(float deltaTime);

public:
	/* --------- 과제 함수 --------- */
	// 충돌 만든다고 귀찮아서 이렇게 만들었습니다. 하하;;;
	void		LevelUp();
	void		GameOver();
	void		GameWin();

	int32		GetLevel();
	int32		GetMonsterCount();
	bool		GetDead();

	void		StartRotate();

	void		SetRotate(float deltaTime);

private:
	int32		_hp = 10;
	float		_speed = 30.0f;
	bool		_isRunning = false;

	bool		_onOff = false;

	bool		_bAssembly = false;

	shared_ptr<Object> _wayObject[5] = {};
	shared_ptr<Object> _moveObject;
	float		_printAngle = 0.0f;

	int32		_level = 2;
	int32		_monsterCount = 0;

	float		_coolTimeMaxRot = 0.0f;
	float		_coolTimeRot = 0.0f;
	bool		_isRot = false;
	bool		_isDead = false;
	float		_coolTimeDead = 0.0f;
	float		_coolTimeMaxDead = 2.0f;

};

/*
OBJECT->FindObject(L"Player")->Attach(Rect, Vec3{ 0.0f, 0.0f, 0.0f });
OBJECT->FindObject(L"Rect")->Attach(Circle, Vec3(0.0f, 50.0f, 0.0f));
OBJECT->FindObject(L"Rect")->Attach(Line_1, Vec3(30.0f, 0.0f, 0.0f));
OBJECT->FindObject(L"Line_1")->Attach(Line_1_1, Vec3(25.0f, 0.0f, 0.0f));
OBJECT->FindObject(L"Rect")->Attach(Line_2, Vec3(-30.0f, 0.0f, 0.0f));
OBJECT->FindObject(L"Line_2")->Attach(Circle_2_1, Vec3(-25.0f, 0.0f, 0.0f));
OBJECT->FindObject(L"Circle_2_1")->Attach(Line_2_1_1, Vec3(0.0f, 0.0f, 0.0f));
OBJECT->FindObject(L"Rect")->Attach(Line_3, Vec3(-20.0f, -30.0f, 0.0f));
OBJECT->FindObject(L"Line_3")->Attach(Line_3_1, Vec3(-10.0f, -40.0f, 0.0f));
OBJECT->FindObject(L"Rect")->Attach(Line_4, Vec3(20.0f, -30.0f, 0.0f));
OBJECT->FindObject(L"Line_4")->Attach(Line_4_1, Vec3(10.0f, -40.0f, 0.0f));
*/