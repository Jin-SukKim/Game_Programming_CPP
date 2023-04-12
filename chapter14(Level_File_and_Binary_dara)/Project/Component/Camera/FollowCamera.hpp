class FollowCamera : public CameraComponent
{
public:
	FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SetHorzDist(float dist) { 
		mHorzDist = dist; 
	}
	void SetVertDist(float dist) {
		mVertDist = dist;
	}
	void SetTargetDist(float dist) {
		mTargetDist = dist;
	}
	void SetSpringConstant(float spring) {
		mSpringConstant = spring;
	}

	// ī�޶� �ʱ�ȭ
	void SnapToIdeal();

	TypeID GetType() const override { return TFollowCamera; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

private:
	Vector3d ComputeCameraPos() const;

	// ī�޶� ��ġ
	Vector3d mActualPos;
	// ī�޶� �ӵ�
	Vector3d mVelocity;
	// ���� �Ÿ�
	float mHorzDist;
	// ���� �Ÿ�
	float mVertDist;
	// Ÿ�� �Ÿ�
	float mTargetDist;
	// ������(ź�¼�)
	float mSpringConstant;
};