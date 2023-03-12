#pragma once
#include "MathUtils.hpp"
#include "Matrix3x3.hpp"

class Vector2d {
public:
	float x;
	float y;

	Vector2d();

	// explicit : �ڵ����� ����ȯ�Ǵ� ���� ���´�.
	explicit Vector2d(float inX, float inY) : x(inX), y(inY) {}

	void Set(float inX, float inY) {
		x = inX;
		y = inY;
	}

	// Vector ���ϱ�
	Vector2d operator+(const Vector2d& v) const {
		return Vector2d(x + v.x, y + v.y);
	}
	// {] �տ� const�� ������ ���ο��� �� ��ȯ�� ���ϹǷ� const�� ���ش�.
	Vector2d& operator+=(const Vector2d& v) {
		// this�� �ڽ��� ����Ű�� �������̴�.
		(*this) = (*this) + v;
		return (*this);
	}

	// Vector ����
	Vector2d operator-(const Vector2d& v) const {
		return Vector2d(x - v.x, y - v.y);
	}
	Vector2d& operator-=(const Vector2d& v) {
		(*this) = (*this) - v;
		return *this;
	}

	// Vector���� ���ϱ�
	Vector2d operator*(const Vector2d& v) const {
		return Vector2d(x * v.x, y * v.y);
	}
	Vector2d& operator*=(const Vector2d& v) {
		(*this) = (*this) * v;
		return *this;
	}
	// Scalar ���ϱ�
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

	// static���� �Լ� ���� �� ��ü�� �������� �ʾƵ� �Լ� ��밡���ϴ�.
	// Normalize the provided vector
	static Vector2d Normalize(const Vector2d& v) {
		Vector2d temp = v;
		temp.Normalize();
		return temp;
	}

	// Dot product(����)
	static float Dot(const Vector2d& a, const Vector2d& b) {
		return (a.x * b.x + a.y * b.y);
	}

	// ���� ����
	static Vector2d Lerp(const Vector2d& a, const Vector2d& b, float f) {
		return Vector2d(a + (b - a) * f);
	}

	// �ݻ� ����
	static Vector2d Reflect(const Vector2d& v, const Vector2d& n) {
		return v - n * 2.f * Vector2d::Dot(v, n);
	}

	// Matrix�� ��ȯ
	static Vector2d Transform(const Vector2d& v, const class Matrix3x3& mat, float w = 1.f);

	static const Vector2d Zero;
	static const Vector2d UnitX;
	static const Vector2d UnitY;
	static const Vector2d NegUnitX;
	static const Vector2d NegUnitY;
};