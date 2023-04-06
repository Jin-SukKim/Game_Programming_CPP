#include "..\..\PreCompiled.hpp"

FollowCamera::FollowCamera(Actor* owner) 
	: CameraComponent(owner), mHorzDist(350.f), mVertDist(150.f)
,mTargetDist(100.f), mSpringConstant(64.f) {}

void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	// ������ ��������κ��� ����(����) ���ڰ� ���
	float dampening = 2.f * MathUtils::Sqrt(mSpringConstant);

	// �̻����� ��ġ ���
	Vector3d idealPos = ComputeCameraPos();

	// �̻����� ��ġ�� ���� ��ġ�� ���� ���
	Vector3d diff = mActualPos - idealPos;
	// �������� ���ӵ��� ���
	// (a = -kx-cv, k = ź�����, x = ���ö �þ ����,
	// c = ����(����) ���, v = �ӵ�)
	Vector3d acel = diff * -mSpringConstant - mVelocity * dampening;

	// �ӵ��� ����
	mVelocity += acel * deltaTime;
	// ���� ī�޶��� ��ġ�� ����
	mActualPos += mVelocity * deltaTime;

	// Ÿ���� ������ �տ� �ִ� ��� �����̴�.
	Vector3d target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	// ī�޶� ���������� ���� ���⿡ ���� ������ UnitZ�̴�.
	Matrix4x4 view = Matrix4x4::CreateLookAt(mActualPos, target,
		Vector3d::UnitZ);
	SetViewMatrix(view);

}

Vector3d FollowCamera::ComputeCameraPos() const
{
	// ī�޶� ��ġ�� �������� �ڿ� �� �밢������ �д�
	Vector3d cameraPos = mOwner->GetPosition();	// OwnerPos
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += Vector3d::UnitZ * mVertDist;
	return cameraPos;
}

void FollowCamera::SnapToIdeal()
{
	// ���� ��ġ�� �̻����� ��ġ�� ����
	mActualPos = ComputeCameraPos();
	// �ӵ��� 0���� �ʱ�ȭ
	mVelocity = Vector3d::Zero;
	// Ÿ�� ������ �� ����� ���
	Vector3d target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	Matrix4x4 view = Matrix4x4::CreateLookAt(mActualPos, target,
		Vector3d::UnitZ);
	SetViewMatrix(view);
}