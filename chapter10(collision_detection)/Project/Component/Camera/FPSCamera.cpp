#include "..\..\PreCompiled.hpp"

FPSCamera::FPSCamera(Actor* owner)
	:CameraComponent(owner), mPitchSpeed(0.f), mMaxPitch(MathUtils::PI / 3.f)
	, mPitch(0.f) {}

void FPSCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	// 카메라의 위치는 소유자의 위치
	Vector3d cameraPos = mOwner->GetPosition();

	// 피치 속도로 피치를 갱신
	mPitch += mPitchSpeed * deltaTime;
	// 피치 값을 [-max, +max] 범위로 제한
	mPitch = MathUtils::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	// 소유자의 오른 축 벡터에 대한 피치 회전을 나타내는 쿼터니언으로 표현
	Quaternion q(mOwner->GetRight(), mPitch);
	// 피치 쿼터니언으로 소유자의 전방 벡터를 회전
	Vector3d viewForward = Vector3d::Transform(
		mOwner->GetForward(), q);
	// 전방 시점 100 단위 앞에 타깃 위치 설정
	Vector3d target = cameraPos + viewForward * 100.f;
	// 피치 쿼터니언으로 상향축도 회전
	Vector3d up = Vector3d::Transform(Vector3d::UnitZ, q);

	// look at 행렬을 생성한 뒤 뷰 행렬로 설정
	Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}