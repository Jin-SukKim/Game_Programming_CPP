#include "PreCompiled.hpp"

OrbitCamera::OrbitCamera(Actor* owner) 
	: CameraComponent(owner), mPitchSpeed(0.f), mYawSpeed(0.f),
	mOffset(-400.f, 0.f, 0.f), mUp(Vector3d::UnitZ) {}

void OrbitCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	// 게임 세계의 상향 벡터와 요 속도를 사용한 쿼터니언 생성
	Quaternion yaw(Vector3d::UnitZ, mYawSpeed * deltaTime);
	// 오프셋과 상향 벡터를 요 쿼터니언을 사용해 변환
	mOffset = Vector3d::Transform(mOffset, yaw);
	mUp = Vector3d::Transform(mUp, yaw);

	// 위의 벡터로부터 카메라의 전방/오른 축 벡터를 계산
	// 전방 벡터 = owner.position - (owner.position + offset)
	// = -offset
	Vector3d forward = mOffset * -1.f;
	forward.Normalize();
	Vector3d right = Vector3d::Cross(mUp, forward);
	right.Normalize();
	// 카메라 오른 축 벡터를 기준으로 회전하는 피치 쿼터니언 생성
	Quaternion pitch(right, mPitchSpeed * deltaTime);
	// 카메라 오프셋과 상향 벡터(카메라의 상향 벡터)를 피치 쿼터니언으로 회전
	mOffset = Vector3d::Transform(mOffset, pitch);
	mUp = Vector3d::Transform(mUp, pitch);

	// 변환 행렬을 계산
	Vector3d target = mOwner->GetPosition();
	Vector3d cameraPos = target + mOffset;
	Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, target, mUp);
	SetViewMatrix(view);

}