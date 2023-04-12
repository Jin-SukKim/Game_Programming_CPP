#include "..\..\PreCompiled.hpp"

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
	// ������Ʈ ������ �ٿ�� �ڽ��� ����
	mWorldBox = mObjectBox;
	// ������
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();
	// ȸ��
	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	// �̵�
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();

}

void BoxComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "objectMin", mObjectBox.mMin);
	JsonHelper::GetVector3(inObj, "objectMax", mObjectBox.mMax);
	JsonHelper::GetVector3(inObj, "worldMin", mWorldBox.mMin);
	JsonHelper::GetVector3(inObj, "worldMax", mWorldBox.mMax);
	JsonHelper::GetBool(inObj, "shouldRotate", mShouldRotate);
}

void BoxComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);

	JsonHelper::AddVector3(alloc, inObj, "objectMin", mObjectBox.mMin);
	JsonHelper::AddVector3(alloc, inObj, "objectMax", mObjectBox.mMax);
	JsonHelper::AddVector3(alloc, inObj, "worldMin", mWorldBox.mMin);
	JsonHelper::AddVector3(alloc, inObj, "worldMax", mWorldBox.mMax);
	JsonHelper::AddBool(alloc, inObj, "shouldRotate", mShouldRotate);
}
