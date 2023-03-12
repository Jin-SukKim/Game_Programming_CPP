#pragma once
#include <algorithm>
#include <vector>
#include "Vector2d.hpp"

class Matrix3x3 {
public:
	float mat[3][3];
	static const Matrix3x3 Identity;

	Matrix3x3();

	explicit Matrix3x3(float inMat[3][3]);

	// float pointer�� ��ȯ
	const float* GetAsFloatPtr() const;

	// Matrix ���ϱ�
	Matrix3x3 operator*(const Matrix3x3& m);

	Matrix3x3& operator*=(const Matrix3x3& m);

	// scale matrix�� �����.(ũ�⺯ȯ)
	static Matrix3x3 CreateScale(float xScale, float yScale);

	static Matrix3x3 CreateScale(const class Vector2d& scaleVector);

	static Matrix3x3 CreateScale(const float scale) {
		return CreateScale(scale, scale);
	}

	// ȸ����ȯ�� ���� matrix
	static Matrix3x3 CreateRotation(float radian);

	// �̵���ȯ(xy-plane)
	static Matrix3x3 CreateTranslation(const class Vector2d& trans);

};