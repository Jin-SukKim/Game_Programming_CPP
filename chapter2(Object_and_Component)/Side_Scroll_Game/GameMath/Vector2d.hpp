#pragma once
#include "MathUtils.hpp"
#include "Matrix3x3.hpp"

class Vector2d {
public:
	float x;
	float y;

	Vector2d();

	// explicit : 자동으로 형변환되는 것을 막는다.
	explicit Vector2d(float inX, float inY) : x(inX), y(inY) {}

	void Set(float inX, float inY) {
		x = inX;
		y = inY;
	}

	// Vector 더하기
	Vector2d operator+(const Vector2d& v) const {
		return Vector2d(x + v.x, y + v.y);
	}
	// {] 앞에 const가 붙으면 내부에서 값 변환을 못하므로 const는 빼준다.
	Vector2d& operator+=(const Vector2d& v) {
		// this란 자신을 가리키는 포인터이다.
		(*this) = (*this) + v;
		return (*this);
	}

	// Vector 빼기
	Vector2d operator-(const Vector2d& v) const {
		return Vector2d(x - v.x, y - v.y);
	}
	Vector2d& operator-=(const Vector2d& v) {
		(*this) = (*this) - v;
		return *this;
	}

	// Vector끼리 곱하기
	Vector2d operator*(const Vector2d& v) const {
		return Vector2d(x * v.x, y * v.y);
	}
	Vector2d& operator*=(const Vector2d& v) {
		(*this) = (*this) * v;
		return *this;
	}
	// Scalar 곱하기
	Vector2d operator*(const float scalar) const {
		return Vector2d(x * scalar, y * scalar);
	}
	Vector2d& operator*=(const float scalar) {
		*this = *this * scalar;
		return *this;
	}

	// Legnth squared of vector
	float LengthSq() const {
		return (x * x + y * y);
	}

	// Length of vector
	float Length() const {
		return (MathUtils::Sqrt(LengthSq()));
	}

	// Normalize vector = vector / size of vector
	void Normalize() {
		float length = Length();
		x /= length;
		y /= length;
	}

	// static으로 함수 선언 시 객체를 생성하지 않아도 함수 사용가능하다.
	// Normalize the provided vector
	static Vector2d Normalize(const Vector2d& v) {
		Vector2d temp = v;
		temp.Normalize();
		return temp;
	}

	// Dot product(내적)
	static float Dot(const Vector2d& a, const Vector2d& b) {
		return (a.x * b.x + a.y * b.y);
	}

	// 선형 보간
	static Vector2d Lerp(const Vector2d& a, const Vector2d& b, float f) {
		return Vector2d(a + (b - a) * f);
	}

	// 반사 벡터
	static Vector2d Reflect(const Vector2d& v, const Vector2d& n) {
		return v - n * 2.f * Vector2d::Dot(v, n);
	}

	// Matrix로 변환
	static Vector2d Transform(const Vector2d& v, const class Matrix3x3& mat, float w = 1.f);

	static const Vector2d Zero;
	static const Vector2d UnitX;
	static const Vector2d UnitY;
	static const Vector2d NegUnitX;
	static const Vector2d NegUnitY;
};