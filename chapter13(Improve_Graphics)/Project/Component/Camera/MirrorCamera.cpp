#include "PreCompiled.hpp"

MirrorCamera::MirrorCamera(Actor* owner)
	:CameraComponent(owner), mHorzDist(150.0f), mVertDist(200.0f),
	mTargetDist(400.0f) {}


void MirrorCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	// �̻����� ��ġ ���
	Vector3d idealPos = ComputeCameraPos();
	// Ÿ���� ������ �տ� �ִ� ��� �����̴�.
	Vector3d target = mOwner->GetPosition() -
		mOwner->GetForward() * mTargetDist;
	// ī�޶� ���������� ���� ���⿡ ���� ������ UnitZ�̴�.
	Matrix4x4 view = Matrix4x4::CreateLookAt(idealPos, target,
		Vector3d::UnitZ);
	// �÷��̾� ���Ϳ� �پ �� ����� �����Ѵ�.
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetMirrorView(view);
}

Vector3d MirrorCamera::ComputeCameraPos() const
{
	// ī�޶� ��ġ ����
	Vector3d cameraPos = mOwner->GetPosition();	// OwnerPos
	// (�������� ������ ������ ����)
	cameraPos += mOwner->GetForward() * mHorzDist;
	cameraPos += Vector3d::UnitZ * mVertDist;
	return cameraPos;
}

void MirrorCamera::SnapToIdeal()
{
	// ���� ��ġ�� �̻����� ��ġ�� ����
	Vector3d idealPos = ComputeCameraPos();
	// Ÿ�� ������ �� ����� ���
	Vector3d target = mOwner->GetPosition() -
		mOwner->GetForward() * mTargetDist;
	Matrix4x4 view = Matrix4x4::CreateLookAt(idealPos, target,
		Vector3d::UnitZ);
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetMirrorView(view);
}