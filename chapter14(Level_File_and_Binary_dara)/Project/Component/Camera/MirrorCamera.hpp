class MirrorCamera : public CameraComponent
{
public:
	MirrorCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }

	TypeID GetType() const override { return TMirrorCamera; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
private:
	// ī�޶� �ʱ�ȭ
	Vector3d ComputeCameraPos() const;

	// ���� �Ÿ�
	float mHorzDist;
	// ���� �Ÿ�
	float mVertDist;
	// Ÿ�� �Ÿ�
	float mTargetDist;
};