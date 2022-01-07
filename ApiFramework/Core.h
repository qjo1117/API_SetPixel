#pragma once

#include "Type.h"


/* -----------
	Core
  ----------- */

class Core
{
public:
	/* �̱� �� */
	static Core* GetInstance() {
		static Core instance;
		return &instance;
	}
public:
	/* ���� �Ҹ� */
	Core();
	~Core();

public:
	/* ���� �Լ� */
	void				SetWindowInfo(WindowInfo info) { _info = info; }
	WindowInfo&			GetWindowInfo() { return _info; }
	const RECTANGLE&	GetRect() { return _info.rect; }

private:
	/* ������ �⺻ �Լ��� */
	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT		WndProc(HWND, UINT, WPARAM, LPARAM);

public:
	/* ������ ��ũ ����� �Ǵ� �Լ��� */
	int					Running(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

	int					Logic();

	bool				Init();
	int					Update(const float& deltaTime);
	int					Render(HDC hdc);
	bool				End();

public:
	void				CreateRobot();

public:
	bool				_isPlayer = false;
	bool				_isEnumys = false;
	list<shared_ptr<class Enemy>> _enumys;

private:
	WindowInfo			_info = {};
	wstring				_tag;

	HDC					_hdc, _memDC;
	HBITMAP				hBitmap, oldBitmap;

	LARGE_INTEGER		_curTime;
	LARGE_INTEGER		_preTime;
	LARGE_INTEGER		_countTime;

public:
	static bool			_bLoop;
};

