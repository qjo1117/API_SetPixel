#include "framework.h"
#include "Core.h"
#include "ObjectManager.h"
#include "ColliderManager.h"
#include "Player.h"
#include "Enemy.h"
#include <stdio.h>
#include <locale.h>
#include <tchar.h>

// ����
bool Core::_bLoop = true;

Core::Core()
{
    // �޸� ������ �÷��׸� ���� üũ�ϴ� �Լ�
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // _CRTDBG_ALLOC_MEM_DF     �Ҵ��ϴ� �޸𸮸� üũ
    // _CRTDBG_LEAK_CHECK_DF    

    //_CrtSetBreakAlloc();
    // �޸� Leak�� �������� ����ϸ� ��

#ifdef _DEBUG
    // �ܼ�â�� �����ϴ� �Լ�.
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
    _info.hInstance = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg = {};

    // ���� �ʱ�ȭ
    Init();

    // �⺻ �޽��� �����Դϴ�:
    while (Core::_bLoop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // ������ ���ư�
        Logic();
    }
    // ����
    End();

    return static_cast<int32>(msg.wParam);
}

int Core::Logic()
{
    // �ӽ� �ڵ� ���ƿ� ������ ���� ���� �ڵ�� �ӽ��ڵ�� �����Ѱ� ����
    QueryPerformanceCounter(&_curTime);

    float deltaTime = (static_cast<float>(_curTime.QuadPart) - static_cast<float>(_preTime.QuadPart)) / static_cast<float>(_countTime.QuadPart);
    _preTime = _curTime;

    Update(deltaTime);      // �̳����
    Render(_hdc); // ����


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

    
    // �Ŵ��� �ʱ�ȭ
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
        /* ----- 2. �� ĳ���� 10���̻� ����� ----- */
        shared_ptr<Enemy> obj = OBJECT->CreateObject<Enemy>();
        Vec3 pos = { random.GetFloatValue(-width,width),random.GetFloatValue(-height,height),0.0f };
        obj->SetPos(pos);
    }

    return true;
}

int Core::Update(const float& deltaTime)
{
    // 1�ʴ� ���� ������ �����Ӹ�ŭ�� Update�� ���������
    // ���� �� ���۸��ϱ��
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
        TextOut(_memDC, _info.rect.right / 2, _info.rect.bottom / 2, L"���콺�� Ŭ�����ּ���", lstrlenW(L"���콺�� Ŭ�����ּ���"));
        
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
