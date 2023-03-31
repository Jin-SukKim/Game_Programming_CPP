#include "..\PreCompiled.hpp"

const Quaternion Quaternion::Identity(0.f, 0.f, 0.f, 1.f);

Quaternion::Quaternion() {
	*this = Quaternion::Identity;
}

Quaternion::Quaternion(float inX, float inY, float inZ, float inW) {
	Set(inX, inY, inZ, inW);
}

// ȸ���� ���Ϳ� ������ ȸ�� ���ʹϾ� �����ϱ�
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
	// ������ ȣ�� ���󰡸� �����ϱ⿡ ������ �ʿ��ϴ�.
	// �������� cos���� ���� ���Ѵ�.
	float rawCosm = Quaternion::Dot(a, b);

	// cos���� ���� < 0�̸� �ݴ�� ȸ��
	float cosVal = -rawCosm;

	// �Ϲ����� ȸ��(�ݽð����)
	if (rawCosm >= 0.f) {
		cosVal = rawCosm;
	}

	float scale0, scale1;

	// cos�� ���� 1�̸� �����ϴٴ� �ǹ��̹Ƿ� �׳� ���� ������ �ϸ� �ȴ�.
	// 1���� ������ ���� ���� ������ ���ش�.
	if (cosVal < 0.9999f) {
		// ������ �����ش�.
		const float omega = MathUtils::Acos(cosVal);
		// �������� ����� �������� �����ɸ��Ƿ� �и� ���� �����ش�.
		const float invSin = 1.f / MathUtils::Sin(omega);
		scale0 = MathUtils::Sin(omega * (1.f - f)) * invSin;
		scale1 = MathUtils::Sin(omega * f) * invSin;
	}
	// �� ���Ͱ� �����ϸ� ���ʹϾ� ���� ������ ���ش�.
	else {
		scale0 = 1.f - f;
		scale1 = f;
	}

	// cos���� ������ �ݴ�� ȸ���ϴ� ���̴�(�ð����)
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

	// �׶󽺸� ���� ���ͺκ�
	// ps * qv + qs * pv + pv x qv
	Vector3d qv(q.x, q.y, q.z);
	Vector3d pv(p.x, p.y, p.z);
	// w���� ��Į�� ���� ����.
	Vector3d newVec = qv * p.w  + pv * q.w  + Vector3d::Cross(pv, qv);
	retVal.x = newVec.x;
	retVal.y = newVec.y;
	retVal.z = newVec.z;

	// �׶󽺸� ���� ��Į�� �κ�
	// ps * qs - pv . qv
	retVal.w = p.w * q.w - Vector3d::Dot(pv, qv);

	return retVal;
}