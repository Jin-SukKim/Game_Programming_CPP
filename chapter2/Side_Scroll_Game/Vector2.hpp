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

		// explicit : �ڵ����� ����ȯ�Ǵ� ���� ���´�.
		explicit Vector2(float inX, float inY) : x(inX), y(inY) {}

		void Set(float inX, float inY) {
			x = inX;
			y = inY;
		}

		// Vector ���ϱ�
		Vector2 operator+(const Vector2& v) const {
			return Vector2(x + v.x, y + v.y);
		}
		// {] �տ� const�� ������ ���ο��� �� ��ȯ�� ���ϹǷ� const�� ���ش�.
		Vector2& operator+=(const Vector2& v) {
			// this�� �ڽ��� ����Ű�� �������̴�.
			(*this) = (*this) + v;
			return (*this);
		}

		// Vector ����
		Vector2 operator-(const Vector2& v) const {
			return Vector2(x - v.x, y - v.y);
		}
		Vector2& operator-=(const Vector2& v) {
			(*this) = (*this) - v;
			return *this;
		}

		// Vector���� ���ϱ�
		Vector2 operator*(const Vector2& v) const {
			return Vector2(x * v.x, y * v.y);
		}
		Vector2& operator*=(const Vector2& v) {
			(*this) = (*this) * v;
			return *this;
		}
		// Scalar ���ϱ�
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

		// static���� �Լ� ���� �� ��ü�� �������� �ʾƵ� �Լ� ��밡���ϴ�.
		// Normalize the provided vector
		static Vector2 Normalize(const Vector2& v) {
			Vector2 temp = v;
			temp.Normalize();
			return temp;
		}

		// Dot product(����)
		static float Dot(const Vector2& a, const Vector2& b) {
			return (a.x * b.x + a.y * b.y);
		}

		// ���� ����
		static Vector2 Lerp(const Vector2& a, const Vector2& b, float f) {
			return Vector2(a + (b - a) * f);
		}

		// �ݻ� ����
		static Vector2 Reflect(const Vector2& v, const Vector2& n) {
			return v - n * 2.f * Vector2::Dot(v, n);
		}

		// Matrix�� ��ȯ
		static Vector2 Transform(const Vector2& v, const Matrix3& mat, float w = 1.f);

		static const Vector2 Zero;
		static const Vector2 UnitX;
		static const Vector2 UnitY;
		static const Vector2 NegUnitX;
		static const Vector2 NegUnitY;
	};
}