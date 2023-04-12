#include "..\..\PreCompiled.hpp"

Matrix4x4 BoneTransform::ToMatrix() const
{
	Matrix4x4 rot = Matrix4x4::CreateFromQuaternion(mRotation);
	Matrix4x4 trans = Matrix4x4::CreateTranslation(mTranslation);

	return rot * trans;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a,
	const BoneTransform& b, float f)
{
	BoneTransform retVal;
	retVal.mRotation = Quaternion::Slerp(a.mRotation, b.mRotation, f);
	retVal.mTranslation = Vector3d::Lerp(a.mTranslation, b.mTranslation, f);
	return retVal;
}