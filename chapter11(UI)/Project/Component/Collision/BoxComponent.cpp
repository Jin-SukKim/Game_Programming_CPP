#include "..\PreCompiled.hpp"

BoxComponent::BoxComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mObjectBox(Vector3d::Zero, Vector3d::Zero)
	, mWorldBox(Vector3d::Zero, Vector3d::Zero)
	, mShouldRotate(true)
{
	mOwner->GetGame()->GetPhysWorld()->AddBox(this);
}

BoxComponent::~BoxComponent()
{
	mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

void BoxComponent::OnUpdateWorldTransform()
{
	// 오브젝트 공간의 바운딩 박스로 리셋
	mWorldBox = mObjectBox;
	// 스케일
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();
	// 회전
	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	// 이동
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();

}