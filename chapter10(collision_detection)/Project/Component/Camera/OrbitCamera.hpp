class OrbitCamera : public CameraComponent
{
public:
	OrbitCamera(class Actor* owner);

	void Update(float deltaTime) override;

	float GetPitchSpeed() const {
		return mPitchSpeed;
	}
	float GetYawSpeed() const {
		return mYawSpeed;
	}

	void SetPitchSpeed(float speed) {
		mPitchSpeed = speed;
	}
	void SetYawSpeed(float speed)
	{
		mYawSpeed = speed;
	}


private:
	// 대상과의 오프셋
	Vector3d mOffset;
	// 카메라의 상향 벡터
	Vector3d mUp;
	// 피치의 초당 회전 속도
	float mPitchSpeed;
	// 요의 초당 회전 속도
	float mYawSpeed;
};