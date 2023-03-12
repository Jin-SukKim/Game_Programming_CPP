#include "Matrix3x3.hpp"

static float m3Ident[3][3] =
{
	{ 1.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f }
};
const Matrix3x3 Matrix3x3::Identity(m3Ident);

Matrix3x3::Matrix3x3() {
	*this = Matrix3x3::Identity;
}

Matrix3x3::Matrix3x3(float inMat[3][3]) {
	std::copy(&mat[0][0], &mat[0][0] + 3 * 3, &inMat[0][0]);
}

// float pointer로 변환
const float* Matrix3x3::GetAsFloatPtr() const {
	// reinterpret_cast : 임의의 포인터 타입끼리 변환을 허용
	return reinterpret_cast<const float*>(&mat[0][0]);
}

// Matrix 곱하기
Matrix3x3 Matrix3x3::operator*(const Matrix3x3& m) {
	Matrix3x3 retVal;
	// dot product(내적)계산과 같다.
	// row 0
	retVal.mat[0][0] =
		mat[0][0] * m.mat[0][0] +
		mat[0][1] * m.mat[1][0] +
		mat[0][2] * m.mat[2][0];

	retVal.mat[0][1] =
		mat[0][0] * m.mat[0][1] +
		mat[0][1] * m.mat[1][1] +
		mat[0][2] * m.mat[2][1];

	retVal.mat[0][2] =
		mat[0][0] * m.mat[0][2] +
		mat[0][1] * m.mat[1][2] +
		mat[0][2] * m.mat[2][2];

	// row 1
	retVal.mat[1][0] =
		mat[1][0] * m.mat[0][0] +
		mat[1][1] * m.mat[1][0] +
		mat[1][2] * m.mat[2][0];

	retVal.mat[1][1] =
		mat[1][0] * m.mat[0][1] +
		mat[1][1] * m.mat[1][1] +
		mat[1][2] * m.mat[2][1];

	retVal.mat[1][2] =
		mat[1][0] * m.mat[0][2] +
		mat[1][1] * m.mat[1][2] +
		mat[1][2] * m.mat[2][2];

	// row 2
	retVal.mat[2][0] =
		mat[2][0] * m.mat[0][0] +
		mat[2][1] * m.mat[1][0] +
		mat[2][2] * m.mat[2][0];

	retVal.mat[2][1] =
		mat[2][0] * m.mat[0][1] +
		mat[2][1] * m.mat[1][1] +
		mat[2][2] * m.mat[2][1];

	retVal.mat[2][2] =
		mat[2][0] * m.mat[0][2] +
		mat[2][1] * m.mat[1][2] +
		mat[2][2] * m.mat[2][2];

	return retVal;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& m) {
	*this = *this * m;
	return *this;
}

Matrix3x3 Matrix3x3::CreateScale(float xScale, float yScale) {
	float temp[3][3] =
	{
		{ xScale, 0.0f, 0.0f },
		{ 0.0f, yScale, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	return Matrix3x3(temp);
}

Matrix3x3 Matrix3x3::CreateScale(const Vector2d& scaleVector) {
	return CreateScale(scaleVector.x, scaleVector.y);
}

Matrix3x3 Matrix3x3::CreateRotation(float radian) {
	float temp[3][3] =
	{
		{ MathUtils::Cos(radian), MathUtils::Sin(radian), 0.0f },
		{ -MathUtils::Sin(radian), MathUtils::Cos(radian), 0.0f },
		{ 0.0f, 0.0f, 1.0f }
	};
	return Matrix3x3(temp);
}


Matrix3x3 Matrix3x3::CreateTranslation(const Vector2d& trans) {
	float temp[3][3] =
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ trans.x, trans.y, 1.0f },
	};
	return Matrix3x3(temp);
}