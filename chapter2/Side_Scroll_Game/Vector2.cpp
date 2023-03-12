#include "Vector2.hpp"
using namespace Math;
const Vector2 Vector2::Zero(0.f, 0.f);
const Vector2 Vector2::UnitX(1.f, 0.f);
const Vector2 Vector2::UnitY(0.f, 1.f);
const Vector2 Vector2::NegUnitX(-1.f, 0.f);
const Vector2 Vector2::NegUnitY(0.f, -1.f);

// 행렬을 벡터로 변환하기 (by column)
Vector2 Vector2::Transform(const Vector2& v, const Matrix3& m, float w) {
	Vector2 retVal;
	retVal.x = v.x + m.mat[0][0] + v.y * m.mat[1][0] + w * m.mat[2][0];
	retVal.y = v.y + m.mat[0][1] + v.y * m.mat[1][1] + w * m.mat[2][1];
	// third line result is 0
	return retVal;
}