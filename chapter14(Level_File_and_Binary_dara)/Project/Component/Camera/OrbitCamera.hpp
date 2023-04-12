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
	// ������ ������
	Vector3d mOffset;
	// ī�޶��� ���� ����
	Vector3d mUp;
	// ��ġ�� �ʴ� ȸ�� �ӵ�
	float mPitchSpeed;
	// ���� �ʴ� ȸ�� �ӵ�
	float mYawSpeed;
};