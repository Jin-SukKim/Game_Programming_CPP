#include "..\PreCompiled.hpp"

const Quaternion Quaternion::Identity(0.f, 0.f, 0.f, 1.f);

Quaternion::Quaternion() {
	*this = Quaternion::Identity;
}

Quaternion::Quaternion(float inX, float inY, float inZ, float inW) {
	Set(inX, inY, inZ, inW);
}

// 회전축 벡터와 각도로 회전 쿼터니언 생성하기
Quaternion::Quaternion(const Vector3d& axis, float angle) {
	float scalar = MathUtils::Sin(angle / 2.f);
	x = axis.x * scalar;
	y = axis.y * scalar;
	z = axis.z * scalar;
	w = MathUtils::Cos(angle / 2.f);
}

float Quaternion::LengthSq() const {
	return (x * x + y * y + z * z + w * w);
}

float Quaternion::Length() const {
	return MathUtils::Sqrt(LengthSq());
}

void Quaternion::Normalize() {
	float length = Length();
	x /= length;
	y /= length;
	z /= length;
	w /= length;
}

Quaternion Quaternion::Normalize(const Quaternion& q) {
	Quaternion retVal = q;
	retVal.Normalize();
	return retVal;
}

Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b,
	float f)
{
	Quaternion retVal;
	retVal.x = MathUtils::Lerp(a.x, b.x, f);
	retVal.y = MathUtils::Lerp(a.y, b.y, f);
	retVal.z = MathUtils::Lerp(a.z, b.z, f);
	retVal.w = MathUtils::Lerp(a.w, b.w, f);
	return retVal;
}

float Quaternion::Dot(const Quaternion& a, const Quaternion& b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b,
	float f)
{
	// 구면의 호를 따라가며 보간하기에 각도가 필요하다.
	// 내적으로 cos각도 값을 구한다.
	float rawCosm = Quaternion::Dot(a, b);

	// cos각도 값이 < 0이면 반대로 회전
	float cosVal = -rawCosm;

	// 일반적인 회전(반시계방향)
	if (rawCosm >= 0.f) {
		cosVal = rawCosm;
	}

	float scale0, scale1;

	// cos의 값이 1이면 평행하다는 의미이므로 그냥 선형 보간을 하면 된다.
	// 1보다 작으면 구면 선형 보간을 해준다.
	if (cosVal < 0.9999f) {
		// 각도를 구해준다.
		const float omega = MathUtils::Acos(cosVal);
		// 나눗셈은 계산이 곱셈보다 오래걸리므로 분모를 먼저 구해준다.
		const float invSin = 1.f / MathUtils::Sin(omega);
		scale0 = MathUtils::Sin(omega * (1.f - f)) * invSin;
		scale1 = MathUtils::Sin(omega * f) * invSin;
	}
	// 두 벡터가 평행하면 쿼터니언 선형 보간을 해준다.
	else {
		scale0 = 1.f - f;
		scale1 = f;
	}

	// cos값이 음수면 반대로 회전하는 것이다(시계방향)
	if (rawCosm < 0.f) {
		scale1 = -scale1;
	}
	
	Quaternion retVal;
	retVal.x = scale0 * a.x + scale1 * b.x;
	retVal.y = scale0 * a.y + scale1 * b.y;
	retVal.z = scale0 * a.z + scale1 * b.z;
	retVal.w = scale0 * a.w + scale1 * b.w;
	retVal.Normalize();
	
	return retVal;
}

Quaternion Quaternion::Concatenate(const Quaternion& q, const Quaternion& p)
{
	Quaternion retVal;

	// 그라스만 곱의 벡터부분
	// ps * qv + qs * pv + pv x qv
	Vector3d qv(q.x, q.y, q.z);
	Vector3d pv(p.x, p.y, p.z);
	// w값은 스칼라 값과 같다.
	Vector3d newVec = qv * p.w  + pv * q.w  + Vector3d::Cross(pv, qv);
	retVal.x = newVec.x;
	retVal.y = newVec.y;
	retVal.z = newVec.z;

	// 그라스만 곱의 스칼라 부분
	// ps * qs - pv . qv
	retVal.w = p.w * q.w - Vector3d::Dot(pv, qv);

	return retVal;
}