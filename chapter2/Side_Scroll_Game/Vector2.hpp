#pragma once
#include <Math.h>
#include "Math.hpp"
#include "Matrix3.hpp"

namespace Math {
	class Vector2 {
	public:
		float x;
		float y;

		Vector2() : x(0.f), y(0.f) {}

		// explicit : 자동으로 형변환되는 것을 막는다.
		explicit Vector2(float inX, float inY) : x(inX), y(inY) {}

		void Set(float inX, float inY) {
			x = inX;
			y = inY;
		}

		// Vector 더하기
		Vector2 operator+(const Vector2& v) const {
			return Vector2(x + v.x, y + v.y);
		}
		// {] 앞에 const가 붙으면 내부에서 값 변환을 못하므로 const는 빼준다.
		Vector2& operator+=(const Vector2& v) {
			// this란 자신을 가리키는 포인터이다.
			(*this) = (*this) + v;
			return (*this);
		}

		// Vector 빼기
		Vector2 operator-(const Vector2& v) const {
			return Vector2(x - v.x, y - v.y);
		}
		Vector2& operator-=(const Vector2& v) {
			(*this) = (*this) - v;
			return *this;
		}

		// Vector끼리 곱하기
		Vector2 operator*(const Vector2& v) const {
			return Vector2(x * v.x, y * v.y);
		}
		Vector2& operator*=(const Vector2& v) {
			(*this) = (*this) * v;
			return *this;
		}
		// Scalar 곱하기
		Vector2 operator*(const float scalar) const {
			return Vector2(x * scalar, y * scalar);
		}
		Vector2& operator*=(const float scalar) {
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
		static Vector2 Normalize(const Vector2& v) {
			Vector2 temp = v;
			temp.Normalize();
			return temp;
		}

		// Dot product(내적)
		static float Dot(const Vector2& a, const Vector2& b) {
			return (a.x * b.x + a.y * b.y);
		}

		// 선형 보간
		static Vector2 Lerp(const Vector2& a, const Vector2& b, float f) {
			return Vector2(a + (b - a) * f);
		}

		// 반사 벡터
		static Vector2 Reflect(const Vector2& v, const Vector2& n) {
			return v - n * 2.f * Vector2::Dot(v, n);
		}

		// Matrix로 변환
		static Vector2 Transform(const Vector2& v, const Matrix3& mat, float w = 1.f);

		static const Vector2 Zero;
		static const Vector2 UnitX;
		static const Vector2 UnitY;
		static const Vector2 NegUnitX;
		static const Vector2 NegUnitY;
	};
}