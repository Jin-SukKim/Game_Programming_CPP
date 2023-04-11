#include "PreCompiled.hpp"

MirrorCamera::MirrorCamera(Actor* owner)
	:CameraComponent(owner), mHorzDist(150.0f), mVertDist(200.0f),
	mTargetDist(400.0f) {}


void MirrorCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	// 이상적인 위치 계산
	Vector3d idealPos = ComputeCameraPos();
	// 타깃은 소유자 앞에 있는 대상 지점이다.
	Vector3d target = mOwner->GetPosition() -
		mOwner->GetForward() * mTargetDist;
	// 카메라가 뒤집혀지는 경우는 업기에 위쪽 방향은 UnitZ이다.
	Matrix4x4 view = Matrix4x4::CreateLookAt(idealPos, target,
		Vector3d::UnitZ);
	// 플레이어 액터에 붙어서 뷰 행렬을 갱신한다.
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetMirrorView(view);
}

Vector3d MirrorCamera::ComputeCameraPos() const
{
	// 카메라 위치 설정
	Vector3d cameraPos = mOwner->GetPosition();	// OwnerPos
	// (소유자의 정면을 보도록 설정)
	cameraPos += mOwner->GetForward() * mHorzDist;
	cameraPos += Vector3d::UnitZ * mVertDist;
	return cameraPos;
}

void MirrorCamera::SnapToIdeal()
{
	// 실제 위치를 이상적인 위치로 설정
	Vector3d idealPos = ComputeCameraPos();
	// 타깃 지점과 뷰 행렬을 계산
	Vector3d target = mOwner->GetPosition() -
		mOwner->GetForward() * mTargetDist;
	Matrix4x4 view = Matrix4x4::CreateLookAt(idealPos, target,
		Vector3d::UnitZ);
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetMirrorView(view);
}