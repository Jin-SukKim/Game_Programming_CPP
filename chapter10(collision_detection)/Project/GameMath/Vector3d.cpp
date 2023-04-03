#include "..\PreCompiled.hpp"

const Vector3d Vector3d::Zero(0.0f, 0.0f, 0.f);
const Vector3d Vector3d::UnitX(1.0f, 0.0f, 0.0f);
const Vector3d Vector3d::UnitY(0.0f, 1.0f, 0.0f);
const Vector3d Vector3d::UnitZ(0.0f, 0.0f, 1.0f);
const Vector3d Vector3d::NegUnitX(-1.0f, 0.0f, 0.0f);
const Vector3d Vector3d::NegUnitY(0.0f, -1.0f, 0.0f);
const Vector3d Vector3d::NegUnitZ(0.0f, 0.0f, -1.0f);
const Vector3d Vector3d::Infinity(MathUtils::Infinity, MathUtils::Infinity, MathUtils::Infinity);
const Vector3d Vector3d::NegInfinity(MathUtils::NegInfinity, MathUtils::NegInfinity, MathUtils::NegInfinity);

Vector3d::Vector3d() : x(0.f), y(0.f), z(0.f) {}
Vector3d::Vector3d(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}

const float* Vector3d::GetAsFloatPtr() const {
	// 포인터로 강제 형변환
	return reinterpret_cast<const float*>(&x);
}

void Vector3d::Set(float inX, float inY, float inZ) {
	x = inX;
	y = inY;
	z = inZ;
}

// 덧셈
Vector3d Vector3d::operator+(const Vector3d& v) const {
	return Vector3d(x + v.x, y + v.y, z + v.z);
}

Vector3d& Vector3d::operator+=(const Vector3d& v) {
	(*this) = (*this) + v;
	return (*this);
}

// 뺄셈
Vector3d Vector3d::operator-(const Vector3d& v) const {
	return Vector3d(x - v.x, y - v.y, z - v.z);
}
Vector3d& Vector3d::operator-=(const Vector3d& v) {
	(*this) = (*this) - v;
	return (*this);
}

// 곱셈
Vector3d Vector3d::operator*(const Vector3d& v) const {
	return Vector3d(x * v.x, y * v.y, z * v.z);
}
Vector3d& Vector3d::operator*=(const Vector3d& v) {
	(*this) = (*this) * v;
	return (*this);
}
Vector3d Vector3d::operator*(float scalar) const {
	return Vector3d(x * scalar, y * scalar, z * scalar);
}
Vector3d& Vector3d::operator*=(float scalar) {
	(*this) = (*this) * scalar;
	return (*this);
}

float Vector3d::LengthSq() const {
	return (x * x + y * y + z * z);
}

float Vector3d::Length() const {
	return (MathUtils::Sqrt(LengthSq()));
}

void Vector3d::Normalize() {
	float length = Length();
	x /= length;
	y /= length;
	z /= length;
}

Vector3d Vector3d::Normalize(const Vector3d& vec) {
	Vector3d temp = vec;
	temp.Normalize();
	return temp;
}

float Vector3d::Dot(const Vector3d& a, const Vector3d& b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

Vector3d Vector3d::Cross(const Vector3d& a, const Vector3d& b) {
	Vector3d temp;
	temp.x = a.y * b.z - a.z * b.y;
	temp.y = a.z * b.x - a.x * b.z;
	temp.z = a.x * b.y - a.y * b.x;
	return temp;
}

Vector3d Vector3d::Lerp(const Vector3d& a, const Vector3d& b, float f) {
	return Vector3d(a + (b - a) * f);
}

Vector3d Vector3d::Reflect(const Vector3d& v, const Vector3d& n)
{
	return v - n * 2.f * Vector3d::Dot(v, n);
}

Vector3d Vector3d::Transform(const Vector3d& vec, const Matrix4x4& mat, float w) {
	Vector3d retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0] + w * mat.mat[3][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1] + w * mat.mat[3][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2] + w * mat.mat[3][2];
	// 이동 벡터의 마지막은 항상 0이 되므로 w는 안해줘도 된다.
	return retVal;
}

// 이동 벡터 그리고 w 값으로 정규화
Vector3d Vector3d::TransformWithPersDiv(const Vector3d& vec, const Matrix4x4& mat, float w) {
	Vector3d retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0] + w * mat.mat[3][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1] + w * mat.mat[3][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2] + w * mat.mat[3][2];
	float transformedW = vec.x * mat.mat[0][3] + vec.y * mat.mat[1][3] +
		vec.z * mat.mat[2][3] + w * mat.mat[3][3];
	if (!MathUtils::NearZero(MathUtils::Abs(transformedW)))
	{
		transformedW = 1.0f / transformedW;
		retVal *= transformedW;
	}
	return retVal;

}

Vector3d Vector3d::Transform(const Vector3d& v, const Quaternion& q) {
	// v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
	Vector3d qv(q.x, q.y, q.z);
	Vector3d retVal = v;
	retVal += Vector3d::Cross(qv, Vector3d::Cross(qv, v) + v * q.w ) * 2.f;
	return retVal;
}