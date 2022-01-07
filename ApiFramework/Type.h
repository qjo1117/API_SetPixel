#pragma once

#include <math.h>
#include <Windows.h>

#define PI 3.141592f
#define RADTODEG(angle) angle * 180.0f / PI
#define DEGTORAD(angle) PI / 180.0f * angle

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

enum class COLLIDER_TYPE
{
	CT_RECT,
	CT_END,
};

/* �� �����ڸ� �Ҷ� ���������� 0.0001������ ��� */
const float Margin = 0.0001f;

/*-----------------------
*		Vector2
-----------------------*/
typedef struct Vector2
{
public:
	Vector2() : x(0.0f),y(0.0f) { }
	Vector2(float fx,float fy) : x(fx),y(fy) { }

	/* �⺻ ���� �Լ��� */
	Vector2 operator+(Vector2& other) {
		return Vector2{ x + other.x, y + other.y };
	}
	Vector2 operator-(Vector2& other) {
		return Vector2{ x - other.x, y - other.y };
	}

	Vector2 operator*(float f) {
		return Vector2{ x * f, y * f };
	}
	Vector2 operator/(float f) {
		return Vector2{ x / f, y / f };
	}

	bool operator==(Vector2& other) {
		return ((other.x - x < Margin) && (other.y - y < Margin) ? true : false);
	}
	bool operator!=(Vector2& other) {
		return !(other == *this);
	}

	float Normalize() {
		return ::sqrtf(Length());
	}
	float Length() {
		return ::powf(x, 2) + ::powf(y, 2);
	}
	Vector2 NormalVector2() {
		float sqr = Normalize();
		return Vector2{ x / sqr,y / sqr };
	}

	/* �ٸ� �����ڵ� */
	static float DotProduct(Vector2& dest, Vector2& src);
	static float Cos(Vector2& dest, Vector2& src);
	static float ArcCos(Vector2& dest, Vector2& src);
public:
	float x, y;
} Vector2D, Vec2;


/*-----------------------
*		Vector3
-----------------------*/
typedef struct Vector3
{
public:
	Vector3() : x(0.0f), y(0.0f),z(0.0f) { }
	Vector3(float fx, float fy, float fz) : x(fx), y(fy), z(fz) { }
	Vector3(const Vector3& other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	/* �⺻ ���� �Լ��� */
	Vector3 operator+(Vector3& other);
	Vector3 operator-(Vector3& other);
	Vector3 operator*(float f);
	Vector3 operator/(float f);

	void operator=(const Vector3& other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	bool operator==(Vector3& other);
	bool operator!=(Vector3& other);

	float		Normalize();
	float		Length();
	Vector3		NormalVector3();

	/* �ٸ� �����ڵ� */
	static float		DotProduct(Vector3& dest, Vector3& src);
	static float		Cos(Vector3& dest, Vector3& src);
	static float		ArcCos(Vector3& dest, Vector3& src);
	static Vector3		CrossProduct(Vector3& dest, Vector3& src);

	static Vector3		Lerp(Vector3& base, Vector3& destination, float delta);
public:
	float x, y, z;
} Vector3D, Vec3;


/*-----------------------
*		Matrix
-----------------------*/
typedef struct Matrix
{
public:
	Matrix() {
		Init();
	}
	Matrix(const Matrix& mat) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				entry[j][i] = mat.entry[j][i];
			}
		}
	}


public:
	/* �����ڵ� */

	// ����� ���
	bool operator==(const Matrix& other) {
		bool check = true;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (other.entry[j][i] - this->entry[j][i] > Margin) {
					check = false;
				}
			}
		}

		return check;
	}
	// �ݴ�
	bool operator!=(const Matrix& other) {
		return !(*this == other);
	}

	// ����� ��
	Matrix operator+(Matrix& other);
	// ����� ����
	Matrix operator-(Matrix& other);
	// ����� ��Į�� ����
	Matrix operator*(float scala);
	// �ǽ��� �帧��� ���� ������
	Matrix operator/(float scala);

	// ���� ������
	void operator=(const Matrix& other) {
		for (int i = 0; i < 16; ++i) {
			this->element[i] = other.element[i];
		}
	}
public:

	/* ���� �� ��ɵ� */
	void Init(float result = 0.f) {
		for (int i = 0; i < 16; ++i) {
			this->element[i] = static_cast<float>(result);
		}
	}
public:
	union
	{
		float entry[4][4];
		float element[16];
		struct {
			float _m11, _m12, _m13, _m14;
			float _m21, _m22, _m23, _m24;
			float _m31, _m32, _m33, _m34;
			float _m41, _m42, _m43, _m44;
		};
	};
} Matrix;


/*-----------------------
*		Vector4
-----------------------*/
typedef struct Vec4 : public Vec3
{
public:
	Vec4() : Vec3(), w(0.0f) { }
	Vec4(float fx, float fy, float fz, float fw = 0.0f) : Vec3(fx, fy, fz), w(fw) { }
	Vec4(Vec4& other) : Vec3(other), w(other.w) { }
	Vec4(Vec3 other) : Vec3(other), w(1.0f) { }

	void operator=(Vec4 other);

	static float DotProduct(Vec4& there, Vec4& other);

public:
	float w;
} Vector4D, Vec4;

// ��Ʈ���� �� �Լ�
Vec4 MultipleMatrixToVector4(Matrix& mat, Vec4& vec);
Vec4 MultipleVector4ToMatrix(Vec4& vec, Matrix& mat);
Matrix MultipleMatrixToMatrix(Matrix& mat1, Matrix& mat2);
float DotProductColumRow(float arr1[4], float arr2[4]);
Matrix TransposeMatrix(Matrix& mat);
Matrix IdentityMatrix(void);

/*--------------------
*	Transform
--------------------*/
void TranslateMatrixColumn(Matrix& mat, Vec3 vec);
void TranslateMatrixRow(Matrix& mat, Vec3 vec);
void RotationMatrixX(Matrix& mat, float angle);
void ScaleMatrix(Matrix& mat, float scale);
Matrix MatrixSRT(float matScale, float matRot, Vec3 matTan = Vec3(0.0f, 0.0f, 0.0f));


/*-----------------------
*		RECTANGLE
-----------------------*/
typedef struct _tagRectangle
{
public:
	_tagRectangle() : left(0.0f),right(0.0f),top(0.0f),bottom(0.0) { }

	Vec2 GetPivotPos(Vec2 pivot = {0.5f,0.5f}) { return Vec2{ (right - left) * pivot.x, (bottom - top) * pivot.y }; }
public:
	float left, right, top, bottom;
} RECTANGLE;


/*-----------------------
*		WindowInfo
-----------------------*/
typedef struct _tagWindowInfo
{
	RECTANGLE rect = {};
	HWND hWnd;
	HINSTANCE hInstance;
} WindowInfo;

/*-----------------------
*		Color
-----------------------*/
typedef struct _tagColor
{
	COLORREF value;
	_tagColor() { value = RGB(0, 0, 0); }
	_tagColor(COLORREF color) { value = color; }

	void SetRed() {
		value = RGB(255, 0, 0);
	}
	void SetBlue() {
		value = RGB(0, 0, 255);
	}
	void SetGreen() {
		value = RGB(0, 0, 255);
	}

} Color;


/*-------------------
*		Quad
-------------------*/
/* ���Ͱ� �����ϴ� �༮ */
typedef struct _tagQuad
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	bool CollisionQRectToPoint(Vec2 pos) {
		if (x - w <= pos.x && pos.x <= x + w) {
			if (y - h <= pos.y && pos.y <= y + h) {
				return true;
			}
		}
		return false;
	}
	bool CollisionQRectToPoint(Vec3 pos) {
		if (x - w <= pos.x && pos.x <= x + w) {
			if (y - h <= pos.y && pos.y <= y + h) {
				return true;
			}
		}
		return false;
	}
} Quad;