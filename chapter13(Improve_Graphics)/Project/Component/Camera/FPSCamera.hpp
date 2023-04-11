class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class Actor* owner);

	void Update(float deltaTime) override;

	float GetPitch() const {
		return mPitch;
	}
	float GetPitchSpeed() const {
		return mPitchSpeed;
	}
	float GetMaxPitch() const {
		return mMaxPitch;
	}

	void SetPitchSpeed(float speed) {
		mPitchSpeed = speed;
	}
	void SetMaxPitch(float pitch) {
		mMaxPitch = pitch;
	}

private:
	// 회전/초(rotation/sec) 속도
	float mPitchSpeed;
	// 최대 피치
	float mMaxPitch;
	// 현재 피치
	float mPitch;
};