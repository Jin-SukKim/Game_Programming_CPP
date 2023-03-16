#include "..\PreCompiled.hpp"

const Vector2d Vector2d::Zero(0.f, 0.f);
const Vector2d Vector2d::UnitX(1.f, 0.f);
const Vector2d Vector2d::UnitY(0.f, 1.f);
const Vector2d Vector2d::NegUnitX(-1.f, 0.f);
const Vector2d Vector2d::NegUnitY(0.f, -1.f);

Vector2d::Vector2d() : x(0.f), y(0.f) {}

// 행렬을 벡터로 변환하기 (by column)
Vector2d Vector2d::Transform(const Vector2d& v, const Matrix3x3& m, float w) {
	Vector2d retVal;
	retVal.x = v.x + m.mat[0][0] + v.y * m.mat[1][0] + w * m.mat[2][0];
	retVal.y = v.y + m.mat[0][1] + v.y * m.mat[1][1] + w * m.mat[2][1];
	// third line result is 0
	return retVal;
}