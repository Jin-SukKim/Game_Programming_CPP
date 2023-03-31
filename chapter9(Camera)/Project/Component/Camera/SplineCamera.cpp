#include "PreCompiled.hpp"

Vector3d Spline::Compute(size_t startIdx, float t) const
{
	// startIdx가 경계값을 초과하는지 검사
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
	
	// p0에서 p3까지의 점을 얻는다.
	Vector3d p0 = mControlPoints[startIdx - 1];
	Vector3d p1 = mControlPoints[startIdx];
	Vector3d p2 = mControlPoints[startIdx + 1];
	Vector3d p3 = mControlPoints[startIdx + 2];

	// 캣멀롬 방정식으로 위치를 계산
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
	// t값 갱신
	if (!mPaused)
	{
		mT += mSpeed * deltaTime;
		// 상황에 따라 다음 제어점으로 이동
		// 카메라 이동 속도는 한 프레임에 복수개의 제어점을 건너뛸 만큼 빠르지 않다고 가정
		if (mT >= 1.f)
		{
			// 경로를 진행하는 데 있어 충분한 점을 가져야 한다.
			if (mIndex < mPath.GetNumPoints() - 3)
			{
				mIndex++;
				mT = mT - 1.f;
			}
			else
			{
				// 경로 진행을 완료했으므로 카메라 이동을 중지시킨다.
				mPaused = true;
			}
		}

		// 카메라 위치는 현재 t/index 값에 해당하는 스플라인에 있다.
		Vector3d cameraPos = mPath.Compute(mIndex, mT);
		// 목표 지점은 t값에 작은 델타값을 더해서 얻은 위치다.
		Vector3d target = mPath.Compute(mIndex, mT + 0.01f);
		// 스플라인 카메라는 거꾸로 뒤집어지지 않는다고 가정
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