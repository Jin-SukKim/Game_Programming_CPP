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

	// float pointer로 변환
	const float* GetAsFloatPtr() const;

	// Matrix 곱하기
	Matrix3x3 operator*(const Matrix3x3& m);

	Matrix3x3& operator*=(const Matrix3x3& m);

	// scale matrix를 만든다.(크기변환)
	static Matrix3x3 CreateScale(float xScale, float yScale);

	static Matrix3x3 CreateScale(const class Vector2d& scaleVector);

	static Matrix3x3 CreateScale(const float scale) {
		return CreateScale(scale, scale);
	}

	// 회변변환을 위한 matrix
	static Matrix3x3 CreateRotation(float radian);

	// 이동변환(xy-plane)
	static Matrix3x3 CreateTranslation(const class Vector2d& trans);

};