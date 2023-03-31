#include "PreCompiled.hpp"

OrbitCamera::OrbitCamera(Actor* owner) 
	: CameraComponent(owner), mPitchSpeed(0.f), mYawSpeed(0.f),
	mOffset(-400.f, 0.f, 0.f), mUp(Vector3d::UnitZ) {}

void OrbitCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	// ���� ������ ���� ���Ϳ� �� �ӵ��� ����� ���ʹϾ� ����
	Quaternion yaw(Vector3d::UnitZ, mYawSpeed * deltaTime);
	// �����°� ���� ���͸� �� ���ʹϾ��� ����� ��ȯ
	mOffset = Vector3d::Transform(mOffset, yaw);
	mUp = Vector3d::Transform(mUp, yaw);

	// ���� ���ͷκ��� ī�޶��� ����/���� �� ���͸� ���
	// ���� ���� = owner.position - (owner.position + offset)
	// = -offset
	Vector3d forward = mOffset * -1.f;
	forward.Normalize();
	Vector3d right = Vector3d::Cross(mUp, forward);
	right.Normalize();
	// ī�޶� ���� �� ���͸� �������� ȸ���ϴ� ��ġ ���ʹϾ� ����
	Quaternion pitch(right, mPitchSpeed * deltaTime);
	// ī�޶� �����°� ���� ����(ī�޶��� ���� ����)�� ��ġ ���ʹϾ����� ȸ��
	mOffset = Vector3d::Transform(mOffset, pitch);
	mUp = Vector3d::Transform(mUp, pitch);

	// ��ȯ ����� ���
	Vector3d target = mOwner->GetPosition();
	Vector3d cameraPos = target + mOffset;
	Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, target, mUp);
	SetViewMatrix(view);

}