#include "..\..\PreCompiled.hpp"

FollowCamera::FollowCamera(Actor* owner) 
	: CameraComponent(owner), mHorzDist(350.f), mVertDist(150.f)
,mTargetDist(100.f), mSpringConstant(100.f) {}

void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	// 스프링 상숫값으로부터 댐핑(감쇄) 인자값 계산
	float dampening = 2.f * MathUtils::Sqrt(mSpringConstant);

	// 이상적인 위치 계산
	Vector3d idealPos = ComputeCameraPos();

	// 이상적인 위치와 실제 위치의 차를 계싼
	Vector3d diff = mActualPos - idealPos;
	// 스프링의 가속도를 계산
	// (a = -kx-cv, k = 탄성계수, x = 용수철 늘어난 길이,
	// c = 댐핑(감쇄) 계수, v = 속도)
	Vector3d acel = diff * -mSpringConstant - mVelocity * dampening;

	// 속도를 갱신
	mVelocity += acel * deltaTime;
	// 실제 카메라의 위치를 갱신
	mActualPos += mVelocity * deltaTime;

	// 타깃은 소유자 앞에 있는 대상 지점이다.
	Vector3d target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	// 카메라가 뒤집혀지는 경우는 업기에 위쪽 방향은 UnitZ이다.
	Matrix4x4 view = Matrix4x4::CreateLookAt(mActualPos, target,
		Vector3d::UnitZ);
	SetViewMatrix(view);

}

Vector3d FollowCamera::ComputeCameraPos() const
{
	// 카메라 위치를 소유자의 뒤와 위 대각선으로 둔다
	Vector3d cameraPos = mOwner->GetPosition();	// OwnerPos
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += Vector3d::UnitZ * mVertDist;
	return cameraPos;
}

void FollowCamera::SnapToIdeal()
{
	// 실제 위치를 이상적인 위치로 설정
	mActualPos = ComputeCameraPos();
	// 속도를 0으로 초기화
	mVelocity = Vector3d::Zero;
	// 타깃 지점과 뷰 행렬을 계산
	Vector3d target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	Matrix4x4 view = Matrix4x4::CreateLookAt(mActualPos, target,
		Vector3d::UnitZ);
	SetViewMatrix(view);
}

void FollowCamera::LoadProperties(const rapidjson::Value& inObj)
{
	CameraComponent::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "actualPos", mActualPos);
	JsonHelper::GetVector3(inObj, "velocity", mVelocity);
	JsonHelper::GetFloat(inObj, "horzDist", mHorzDist);
	JsonHelper::GetFloat(inObj, "vertDist", mVertDist);
	JsonHelper::GetFloat(inObj, "targetDist", mTargetDist);
	JsonHelper::GetFloat(inObj, "springConstant", mSpringConstant);
}

void FollowCamera::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	CameraComponent::SaveProperties(alloc, inObj);

	JsonHelper::AddVector3(alloc, inObj, "actualPos", mActualPos);
	JsonHelper::AddVector3(alloc, inObj, "velocity", mVelocity);
	JsonHelper::AddFloat(alloc, inObj, "horzDist", mHorzDist);
	JsonHelper::AddFloat(alloc, inObj, "vertDist", mVertDist);
	JsonHelper::AddFloat(alloc, inObj, "targetDist", mTargetDist);
	JsonHelper::AddFloat(alloc, inObj, "springConstant", mSpringConstant);
}