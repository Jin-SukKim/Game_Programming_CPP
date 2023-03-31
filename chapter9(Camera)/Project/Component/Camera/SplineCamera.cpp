#include "PreCompiled.hpp"

Vector3d Spline::Compute(size_t startIdx, float t) const
{
	// startIdx�� ��谪�� �ʰ��ϴ��� �˻�
	if (startIdx >= mControlPoints.size())
	{
		return mControlPoints.back();
	}
	else if (startIdx == 0)
	{
		return mControlPoints[startIdx];
	}
	else if (startIdx + 2 >= mControlPoints.size())
	{
		return mControlPoints[startIdx];
	}
	
	// p0���� p3������ ���� ��´�.
	Vector3d p0 = mControlPoints[startIdx - 1];
	Vector3d p1 = mControlPoints[startIdx];
	Vector3d p2 = mControlPoints[startIdx + 1];
	Vector3d p3 = mControlPoints[startIdx + 2];

	// Ĺ�ַ� ���������� ��ġ�� ���
	Vector3d position = ((p1 * 2.0f) + (p0 * -1.0f + p2) * t +
		(p0 * 2.0f - p1 * 5.0f + p2 * 4.0f - p3) * t * t +
		(p0 * -1.0f + p1 * 3.0f - p2 * 3.0f + p3) * t * t * t) * 0.5f;
	return position;

}

SplineCamera::SplineCamera(Actor* owner)
	:CameraComponent(owner), mIndex(1), mT(0.f), mSpeed(0.5f), mPaused(true) {}

void SplineCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	// t�� ����
	if (!mPaused)
	{
		mT += mSpeed * deltaTime;
		// ��Ȳ�� ���� ���� ���������� �̵�
		// ī�޶� �̵� �ӵ��� �� �����ӿ� �������� �������� �ǳʶ� ��ŭ ������ �ʴٰ� ����
		if (mT >= 1.f)
		{
			// ��θ� �����ϴ� �� �־� ����� ���� ������ �Ѵ�.
			if (mIndex < mPath.GetNumPoints() - 3)
			{
				mIndex++;
				mT = mT - 1.f;
			}
			else
			{
				// ��� ������ �Ϸ������Ƿ� ī�޶� �̵��� ������Ų��.
				mPaused = true;
			}
		}

		// ī�޶� ��ġ�� ���� t/index ���� �ش��ϴ� ���ö��ο� �ִ�.
		Vector3d cameraPos = mPath.Compute(mIndex, mT);
		// ��ǥ ������ t���� ���� ��Ÿ���� ���ؼ� ���� ��ġ��.
		Vector3d target = mPath.Compute(mIndex, mT + 0.01f);
		// ���ö��� ī�޶�� �Ųٷ� ���������� �ʴ´ٰ� ����
		const Vector3d up = Vector3d::UnitZ;
		Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, target, up);
		SetViewMatrix(view);
	}
}

void SplineCamera::Restart()
{
	mIndex = 1;
	mT = 0.f;
	mPaused = false;
}