#include "framework.h"
#include "Core.h"
#include "ObjectManager.h"
#include "ColliderManager.h"
#include "Player.h"
#include "Enemy.h"
#include <stdio.h>
#include <locale.h>
#include <tchar.h>

// 선언
bool Core::_bLoop = true;

Core::Core()
{
    // 메모리 누수를 플레그를 통해 체크하는 함수
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // _CRTDBG_ALLOC_MEM_DF     할당하는 메모리를 체크
    // _CRTDBG_LEAK_CHECK_DF    

    //_CrtSetBreakAlloc();
    // 메모리 Leak이 있을때만 사용하면 됨

#ifdef _DEBUG
    // 콘솔창을 생성하는 함수.
    AllocConsole();

#endif // _DEBUG
}

Core::~Core()
{
    FreeConsole();
}

ATOM Core::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Core::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = _tag.c_str();
    wcex.hIconSm = 0;

    return RegisterClassExW(&wcex);
}

BOOL Core::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    _info.hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    _info.hWnd = CreateWindowW(_tag.c_str(), _tag.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!_info.hWnd)
    {
        return FALSE;
    }

    ShowWindow(_info.hWnd, nCmdShow);
    UpdateWindow(_info.hWnd);

    return TRUE;
}

LRESULT CALLBACK Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int Core::Running(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    _tag = L"Graphic";

    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg = {};

    // 게임 초기화
    Init();

    // 기본 메시지 루프입니다:
    while (Core::_bLoop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // 로직이 돌아감
        Logic();
    }
    // 종료
    End();

    return static_cast<int32>(msg.wParam);
}

int Core::Logic()
{
    // 임시 코드 정훈영 교수님 제출 전용 코드라서 임시코드로 땜빵한게 많음
    QueryPerformanceCounter(&_curTime);

    float deltaTime = (static_cast<float>(_curTime.QuadPart) - static_cast<float>(_preTime.QuadPart)) / static_cast<float>(_countTime.QuadPart);
    _preTime = _curTime;

    Update(deltaTime);      // 이놈먼저
    Render(_hdc); // 그후


    return 0;
}

bool Core::Init()
{
    _hdc = GetDC(_info.hWnd);
    RECT client = {};
    GetClientRect(_info.hWnd, &client);
    _info.rect.left = static_cast<float>(client.left);
    _info.rect.right = static_cast<float>(client.right);
    _info.rect.top = static_cast<float>(client.top);
    _info.rect.bottom = static_cast<float>(client.bottom);

    QueryPerformanceFrequency(&_countTime);
    QueryPerformanceCounter(&_curTime);
    QueryPerformanceCounter(&_preTime);

    
    // 매니저 초기화
    if (OBJECT->Init() == 1) {
        MessageBox(nullptr, L"Hello Objet", L"WOW", MB_OK);
    }
    if (COLLIDER->Init(CORE->GetRect().right, CORE->GetRect().bottom) == 1) {
        MessageBox(nullptr, L"Hello Collder", L"WOW", MB_OK);
    }

    Random random;
    random.Init(0);

    float width = CORE->GetRect().right / 2;
    float height = CORE->GetRect().bottom / 2;

    for (int32 i = 0; i < 10; ++i) {
        /* ----- 2. 적 캐릭터 10개이상 만들기 ----- */
        shared_ptr<Enemy> obj = OBJECT->CreateObject<Enemy>();
        Vec3 pos = { random.GetFloatValue(-width,width),random.GetFloatValue(-height,height),0.0f };
        obj->SetPos(pos);
    }

    return true;
}

int Core::Update(const float& deltaTime)
{
    // 1초당 일정 고정된 프레임만큼만 Update를 시켜줘야함
    // 조금 더 구글링하기로
    COLLIDER->Update(deltaTime);
    
    OBJECT->Update(deltaTime);

    if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        if (_isPlayer == false) {
            _isPlayer = true;
            OBJECT->CreateObject<Player>();
        }
    }

    return 0;
}

int Core::Render(HDC hdc)
{
    _memDC = ::CreateCompatibleDC(hdc);
    hBitmap = ::CreateCompatibleBitmap(hdc, static_cast<int>(CORE->GetRect().right), static_cast<int>(CORE->GetRect().bottom));
    oldBitmap = (HBITMAP)SelectObject(_memDC, hBitmap);
    PatBlt(_memDC, 0, 0, static_cast<int>(CORE->GetRect().right), static_cast<int>(CORE->GetRect().bottom), WHITENESS);

    // -------------------------------------------

    OBJECT->Render(_memDC);
    COLLIDER->Render(_memDC);

    if (_isPlayer == false) {
        TextOut(_memDC, _info.rect.right / 2, _info.rect.bottom / 2, L"마우스를 클릭해주세요", lstrlenW(L"마우스를 클릭해주세요"));
        
    }

    // -------------------------------------------

    BitBlt(hdc, 0, 0, static_cast<int>(CORE->GetRect().right), static_cast<int>(CORE->GetRect().bottom), _memDC, 0, 0, SRCCOPY);
    SelectObject(_memDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(_memDC);

    return 0;
}

bool Core::End()
{
    OBJECT->End();
    COLLIDER->End();

    ReleaseDC(_info.hWnd, _hdc);

    return false;
}

void Core::CreateRobot()
{
}
