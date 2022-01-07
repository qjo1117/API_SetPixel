#include "framework.h"
#include "Type.h"
#include <math.h>

float Vector2::DotProduct(Vector2& there, Vector2& other)
{
	return (there.x * other.x) + (there.y * other.y);
}

float Vector2::Cos(Vector2& dest, Vector2& src)
{
	float dot = Vector2::DotProduct(dest, src);
	float ab = dest.Normalize() * src.Normalize();
	return dot / ab;
}

float Vector2::ArcCos(Vector2& dest, Vector2& src)
{
	return ::acosf(Vector2::Cos(dest, src));
}

//Vector2 Vector2::CrossProduct(Vector2& dest, Vector2& src)
//{
//    return (dest.y * src.z) - (dest.z)
//}
////(a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1)  
Matrix Matrix::operator+(Matrix& other)
{
	Matrix temp;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			temp.entry[j][i] = this->entry[j][i] + other.entry[i][j];
		}
	}
	return temp;
}
Matrix Matrix::operator-(Matrix& other)
{
	Matrix temp;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			temp.entry[j][i] = this->entry[j][i] - other.entry[j][i];
		}
	}
	return temp;
}

Matrix Matrix::operator*(float scala)
{
	Matrix temp;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			temp.entry[j][i] = this->entry[j][i] * scala;
		}
	}
	return temp;
}

Matrix Matrix::operator/(float scala)
{
	Matrix temp;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			temp.entry[j][i] = this->entry[j][i] / scala;
		}
	}
	return temp;
}

Vec4 MultipleMatrixToVector4(Matrix& mat, Vec4& vec)
{
	float fTemp[4] = { 0.0f, };

	for (int i = 0; i < 4; ++i) {
		fTemp[i] += vec.x * mat.entry[i][0];
		fTemp[i] += vec.y * mat.entry[i][1];
		fTemp[i] += vec.z * mat.entry[i][2];
		fTemp[i] += vec.w * mat.entry[i][3];
	}
	Vec4 vTemp(fTemp[0], fTemp[1], fTemp[2], fTemp[3]);
	return vTemp;
}

Vec4 MultipleVector4ToMatrix(Vec4& vec, Matrix& mat)
{
	float fTemp[4] = { 0.0f, };

	for (int32 i = 0; i < 4; ++i) {
		fTemp[i] += vec.x * mat.entry[0][i];
		fTemp[i] += vec.y * mat.entry[1][i];
		fTemp[i] += vec.z * mat.entry[2][i];
		fTemp[i] += vec.w * mat.entry[3][i]; 
	 }
	Vec4 vTemp(fTemp[0], fTemp[1], fTemp[2], fTemp[3]);
	return vTemp;
}

Matrix MultipleMatrixToMatrix(Matrix& mat1, Matrix& mat2)
{
	Matrix mTemp;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			mTemp.entry[i][j] = DotProductColumRow(&mat1.entry[i][0], &mat2.entry[0][j]);
		}
	}
	return mTemp;
}

float DotProductColumRow(float arr1[4], float arr2[4])
{
	float result = 0.0f;
	for (int i = 0; i < 4; ++i) {
		result += arr1[i] * arr2[4 * i];
	}
	return result;
}

Matrix TransposeMatrix(Matrix& mat)
{
	Matrix matTemp;
	matTemp.Init();
	for (int32 i = 0; i < 4; ++i) {
		for (int32 j = 0; j < 4; ++j) {
			matTemp.entry[j][i] = mat.entry[i][j];
		}
	}

	return matTemp;
}

Matrix IdentityMatrix(void)
{
	Matrix matTemp;
	matTemp.Init(0);
	for (int i = 0; i < 4; ++i) {
		matTemp.entry[i][i] = 1;
	}
	return matTemp;
}

void TranslateMatrixColumn(Matrix& mat, Vec3 vec)
{
	mat.entry[3][0] += vec.x;
	mat.entry[3][1] += vec.y;
	mat.entry[3][2] += vec.z;
	mat.entry[3][3] = 1.0f;
}

void TranslateMatrixRow(Matrix& mat, Vec3 vec)
{
	mat.entry[0][3] += vec.x;
	mat.entry[1][3] += vec.y;
	mat.entry[2][3] += vec.z;
	mat.entry[3][3] = 1.0f;
}

void RotationMatrixX(Matrix& mat, float angle)
{
	mat.entry[0][0] = cosf(DEGTORAD(angle));
	mat.entry[0][1] = -sinf(DEGTORAD(angle));
	mat.entry[1][0] = sinf(DEGTORAD(angle));
	mat.entry[1][1] = cosf(DEGTORAD(angle));
}

void ScaleMatrix(Matrix& mat, float scale)
{
	mat.entry[0][0] = scale;
	mat.entry[1][1] = scale;
	mat.entry[2][2] = scale;
}

Matrix MatrixSRT(float scale, float rotation, Vec3 translation)
{
	Matrix matTranslation = IdentityMatrix();
	Matrix matRotation = IdentityMatrix();
	Matrix matScale = IdentityMatrix();

	Matrix mat = IdentityMatrix();

	Vec4 tempTran = translation;

	ScaleMatrix(matScale, scale);
	RotationMatrixX(matRotation, rotation);
	TranslateMatrixRow(matTranslation, tempTran);

	mat = MultipleMatrixToMatrix(matScale, matRotation);
	mat = MultipleMatrixToMatrix(mat, matTranslation);

	return mat;
}



Vector3 Vector3::operator+(Vector3& other)
{
	return Vector3{ x + other.x, y + other.y, z + other.z };
}

Vector3 Vector3::operator-(Vector3& other)
{
	return Vector3{ x - other.x, y - other.y, z - other.z };
}

Vector3 Vector3::operator*(float f)
{
	return Vector3{ x * f, y * f, z * f };
}

Vector3 Vector3::operator/(float f)
{
	return Vector3{ x / f, y / f, z / f };
}

bool Vector3::operator==(Vector3& other)
{
	return (x == other.x) && (y == other.y) && (z == other.z) ? true : false;
}

bool Vector3::operator!=(Vector3& other)
{
	return !(*this == other);
}

float Vector3::Normalize()
{
	return ::sqrtf(Length());
}

float Vector3::Length()
{
	return ::powf(x, 2) + ::powf(y, 2) + ::powf(z, 2);
}

Vector3 Vector3::NormalVector3()
{
	float sqr = Normalize();
	return Vector3{ x / sqr,y / sqr, z / sqr };
}

float Vector3::DotProduct(Vector3& dest, Vector3& src)
{
	return (dest.x * src.x) + (dest.y * src.y) + (dest.z * src.z);
}

float Vector3::Cos(Vector3& dest, Vector3& src)
{
	return Vector3::DotProduct(dest, src) / (dest.Normalize() * src.Normalize());
}

float Vector3::ArcCos(Vector3& dest, Vector3& src)
{
	return ::acosf(Vector3::Cos(dest, src));
}

Vector3 Vector3::CrossProduct(Vector3& dest, Vector3& src)
{
	// ±ÍÂúÀ½
	return Vector3();
}

Vector3 Vector3::Lerp(Vector3& base, Vector3& dest, float alpha)
{
	Vector3 lerp = (base - dest) * alpha;
	return base + lerp;
}

void Vec4::operator=(Vec4 other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}

float Vec4::DotProduct(Vec4& there, Vec4& other)
{
	return Vec3::DotProduct(there, other) + (there.w * other.w);
}