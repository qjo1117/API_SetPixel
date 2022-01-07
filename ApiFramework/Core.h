#pragma once

#include "Type.h"


/* -----------
	Core
  ----------- */

class Core
{
public:
	/* 싱글 톤 */
	static Core* GetInstance() {
		static Core instance;
		return &instance;
	}
public:
	/* 생성 소멸 */
	Core();
	~Core();

public:
	/* 헬퍼 함수 */
	void				SetWindowInfo(WindowInfo info) { _info = info; }
	WindowInfo&			GetWindowInfo() { return _info; }
	const RECTANGLE&	GetRect() { return _info.rect; }

private:
	/* 윈도우 기본 함수들 */
	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT		WndProc(HWND, UINT, WPARAM, LPARAM);

public:
	/* 프레임 워크 기반이 되는 함수들 */
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

