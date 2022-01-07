#include "framework.h"
#include "Player.h"
#include "Shape.h"
#include "Core.h"
#include "RectCollider.h"

#include "Core.h"
#include "ObjectManager.h"
#include "ColliderManager.h"

/* -----------
    Player
  ----------- */

Player::Player() : Object()
{
}

Player::Player(Player& other) :
    Object(other),
	_hp(other._hp),
	_speed(other._speed)
{
}

bool Player::Init()
{
    _tag = L"Player";

    _angle = 0.0f;

    /* -- 1. �÷��̾� ĳ���� - �κ� O / �簢�� (�Ķ���) -- */
    CreatePart();
    Assembly();
    SetColor(RGB(0, 0, 255));

    // Collider ����
    shared_ptr<RectCollider> collider = COLLIDER->CreateCollider<RectCollider>(shared_from_this());
    collider->SetWidth((_scale - 0.1f) * 100);
    collider->SetHeight((_scale - 0.1f) * 90);

    _level = 0;
    LevelUp();

	return true;
}

int Player::Update(float deltaTime)
{
	Object::Update(deltaTime);

    CheckDead(deltaTime);
    InputMove(deltaTime);

	return 0;
}

bool Player::Render(HDC hdc)
{
    Object::Render(hdc);


	return true;
}

bool Player::End()
{
	Object::End();

	return false;
}

void Player::RotationObject(const wstring& name, float angle)
{
    shared_ptr<Object> objTemp = FindChild(name);
    if (objTemp == nullptr) {
        return;
    }
    objTemp->SetAngle(objTemp->GetAngle() + angle);
}

bool Player::Assembly()
{
    weak_ptr<Object> obj = OBJECT->FindObject(L"Rect");
    OBJECT->FindObject(L"Player")->Attach(obj.lock(), Vec3{ 0.0f, 0.0f, 0.0f });
    obj = OBJECT->FindObject(L"Circle");
    OBJECT->FindObject(L"Rect")->Attach(obj.lock(), Vec3(0.0f, 50.0f, 0.0f));
    obj = OBJECT->FindObject(L"Line_1");
    OBJECT->FindObject(L"Rect")->Attach(obj.lock(), Vec3(30.0f, 0.0f, 0.0f));
    obj = OBJECT->FindObject(L"Line_1_1");
    OBJECT->FindObject(L"Line_1")->Attach(obj.lock(), Vec3(25.0f, 0.0f, 0.0f));
    obj = OBJECT->FindObject(L"Line_2");
    OBJECT->FindObject(L"Rect")->Attach(obj.lock(), Vec3(-30.0f, 0.0f, 0.0f));
    obj = OBJECT->FindObject(L"Circle_2_1");
    OBJECT->FindObject(L"Line_2")->Attach(obj.lock(), Vec3(-25.0f, 0.0f, 0.0f));
    obj = OBJECT->FindObject(L"Line_2_1_1");
    OBJECT->FindObject(L"Circle_2_1")->Attach(obj.lock(), Vec3(0.0f, 0.0f, 0.0f));
    obj = OBJECT->FindObject(L"Line_3");
    OBJECT->FindObject(L"Rect")->Attach(obj.lock(), Vec3(-20.0f, -30.0f, 0.0f));
    obj = OBJECT->FindObject(L"Line_3_1");
    OBJECT->FindObject(L"Line_3")->Attach(obj.lock(), Vec3(-10.0f, -40.0f, 0.0f));
    obj = OBJECT->FindObject(L"Line_4");
    OBJECT->FindObject(L"Rect")->Attach(obj.lock(), Vec3(20.0f, -30.0f, 0.0f));
    obj = OBJECT->FindObject(L"Line_4_1");
    OBJECT->FindObject(L"Line_4")->Attach(obj.lock(), Vec3(10.0f, -40.0f, 0.0f));

    obj = OBJECT->FindObject(L"Direction");
    OBJECT->FindObject(L"Rect")->Attach(obj.lock(), Vec3(0.0f, 0.0f, 0.0f));
    return true;
}

void Player::CreatePart()
{
    // ����
    {
        shared_ptr<Rect> obj = OBJECT->CreateObject<Rect>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetRect(30, 30);
        obj->SetTag(L"Rect");
        obj->SetColor(RGB(0, 0, 255));
    }

    // �Ӹ�
    {
        shared_ptr<Circle> obj = OBJECT->CreateObject<Circle>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetRadius(20.0f);
        obj->SetTag(L"Circle");
    }

    // �� (��)
    {
        shared_ptr<Line> obj = OBJECT->CreateObject<Line>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetStart(0.0f, 0.0f);
        obj->SetEnd(25.0f, 10.0f);
        obj->SetTag(L"Line_1");
    }
    // �� (��_�μ�_��)
    {
        shared_ptr<Line> obj = OBJECT->CreateObject<Line>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetStart(0.0f, 0.0f);
        obj->SetEnd(25.0f, 100.0f);
        obj->SetTag(L"Line_1_1");
        obj->SetColor(RGB(100, 100, 100));
    }

    // �� (��)
    {

        shared_ptr<Line> obj = OBJECT->CreateObject<Line>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetStart(0.0f, 0.0f);
        obj->SetEnd(-25.0f, 10.0f);
        obj->SetTag(L"Line_2");
        obj->SetColor(Color{ RGB(255,0,0) });

    }
    // �� (��_�μ�_����)
    {
        shared_ptr<Circle> obj = OBJECT->CreateObject<Circle>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetRadius(30.0f);
        obj->SetTag(L"Circle_2_1");
        obj->SetColor(Color{ RGB(255,0,0) });
    }
    // �� (��_�μ�_��)
    {
        shared_ptr<Line> obj = OBJECT->CreateObject<Line>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetStart(0.0f, 0.0f);
        obj->SetEnd(0.0f, -50.0f);
        obj->SetTag(L"Line_2_1_1");
        obj->SetColor(Color{ RGB(0,255,0) });
    }

    // �ٸ� (��)
    {
        shared_ptr<Line> obj = OBJECT->CreateObject<Line>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetStart(0.0f, 0.0f);
        obj->SetEnd(-10.0f, -40.0f);
        obj->SetTag(L"Line_3");
        obj->SetColor(Color{ RGB(255,0,0) });
    }
    // �ٸ� (��_�μ�_1)
    {
        shared_ptr<Line> obj = OBJECT->CreateObject<Line>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetStart(0.0f, 0.0f);
        obj->SetEnd(-10.0f, 0.0f);
        obj->SetTag(L"Line_3_1");
        obj->SetColor(Color{ RGB(255,0,0) });
    }

    // �ٸ� (��)
    {
        shared_ptr<Line> obj = OBJECT->CreateObject<Line>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetStart(0.0f, 0.0f);
        obj->SetEnd(10.0f, -40.0f);
        obj->SetTag(L"Line_4");
    }
    // �ٸ� (��_�μ�_1)
    {
        shared_ptr<Line> obj = OBJECT->CreateObject<Line>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetStart(0.0f, 0.0f);
        obj->SetEnd(10.0f, 0.0f);
        obj->SetTag(L"Line_4_1");
    }
    // Direction
    {
        shared_ptr<Line> obj = OBJECT->CreateObject<Line>();
        obj->SetPos(0.0f, 0.0f, 0.0f);
        obj->SetStart(0.0f, 0.0f);
        obj->SetEnd(0.0f, 20.0f);
        obj->SetColor(RGB(0, 125, 200));
        obj->SetTag(L"Direction");
    }
}

void Player::InputMove(float deltaTime)
{
    /* ------- 4. WSAD�� �̿��ؼ� �÷��̾ 8�������� ������ ---------*/
    _isRunning = false;
    if (::GetAsyncKeyState(VK_LSHIFT) & 0x8000) {
        _isRunning = true;
    }

    float accel = 3.0f;

    if (::GetAsyncKeyState('W') & 0x8000) {
        _isRunning == false ? _move.y += _speed * deltaTime : _move.y += _speed * accel * deltaTime;
    }
    if (::GetAsyncKeyState('S') & 0x8000) {
        _isRunning == false ? _move.y -= _speed * deltaTime : _move.y -= _speed * accel * deltaTime;
    }
    if (::GetAsyncKeyState('D') & 0x8000) {
        _isRunning == false ? _move.x += _speed * deltaTime : _move.x += _speed * accel * deltaTime;
    }
    if (::GetAsyncKeyState('A') & 0x8000) {
        _isRunning == false ? _move.x -= _speed * deltaTime : _move.x -= _speed * accel * deltaTime;
    }

    SetRotate(deltaTime);

    Matrix mat = IdentityMatrix();
    TranslateMatrixRow(mat, _move);
    Vec4 move(_move);
    _pos = MultipleMatrixToVector4(mat, move);
}

void Player::CheckDead(float deltaTime)
{
    /* ----------- �׾����� ------------- */
    if (_isDead == true) {
        _coolTimeDead += deltaTime;

        _angle += 720.0f * deltaTime;
        _scale -= (_level * 0.05f) * deltaTime;
        if (_scale <= 0.01f) {
            _scale = 0.01f;
        }

        if (_coolTimeMaxDead < _coolTimeDead) {
            SetActive(false);
            MessageBox(nullptr, L"Game Over", L"Game Fail", MB_OK);
            Core::_bLoop = false;
        }
    }
}

void Player::LevelUp()
{
    /* ���� �κ� */
    _level += 1;
    _monsterCount += 1;

    switch (_level) {
    case 1:
        _scale = 0.3f;
        break;

    case 2:
        _scale = 0.4f;
        break;

    case 3:
        _scale = 0.5f;
        break;

    case 4:
        _scale = 0.6f;
        break;

    case 5:
        _scale = 0.7f;
        break;

    case 6:
        _scale = 1.0f;
        break;
    }

    shared_ptr<RectCollider> collider = static_pointer_cast<RectCollider>(_collider);
    collider->SetWidth((_scale - 0.1f) * 100);
    collider->SetHeight((_scale - 0.1f) * 90);
}

void Player::GameOver()
{
    _isDead = true;
}

void Player::GameWin()
{
    /* ���� �¸� */
    MessageBox(nullptr, L"Game Clear", L"Game Over", MB_OK);
    Core::_bLoop = false;
}

int32 Player::GetLevel()
{
    return _level;
}

int32 Player::GetMonsterCount()
{
    return _monsterCount;
}

bool Player::GetDead()
{
    return _isDead;
}

void Player::StartRotate()
{
    _isRot = true;
    _coolTimeMaxRot += 1.0f;
}

void Player::SetRotate(float deltaTime)
{
    if (_isRot == true) {
        _coolTimeRot += deltaTime;

        _angle += 360.0f * deltaTime;
        if (_coolTimeMaxRot <= _coolTimeRot) {
            _isRot = false;
            return;
        }
    }
}

