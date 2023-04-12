#include "..\..\PreCompiled.hpp"

FPSCamera::FPSCamera(Actor* owner)
	:CameraComponent(owner), mPitchSpeed(0.f), mMaxPitch(MathUtils::PI / 3.f)
	, mPitch(0.f) {}

void FPSCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	// ī�޶��� ��ġ�� �������� ��ġ
	Vector3d cameraPos = mOwner->GetPosition();

	// ��ġ �ӵ��� ��ġ�� ����
	mPitch += mPitchSpeed * deltaTime;
	// ��ġ ���� [-max, +max] ������ ����
	mPitch = MathUtils::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	// �������� ���� �� ���Ϳ� ���� ��ġ ȸ���� ��Ÿ���� ���ʹϾ����� ǥ��
	Quaternion q(mOwner->GetRight(), mPitch);
	// ��ġ ���ʹϾ����� �������� ���� ���͸� ȸ��
	Vector3d viewForward = Vector3d::Transform(
		mOwner->GetForward(), q);
	// ���� ���� 100 ���� �տ� Ÿ�� ��ġ ����
	Vector3d target = cameraPos + viewForward * 100.f;
	// ��ġ ���ʹϾ����� �����൵ ȸ��
	Vector3d up = Vector3d::Transform(Vector3d::UnitZ, q);

	// look at ����� ������ �� �� ��ķ� ����
	Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}