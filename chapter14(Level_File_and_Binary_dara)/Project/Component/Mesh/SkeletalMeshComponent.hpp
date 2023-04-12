class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);
	// �޽� ������Ʈ�� �׸���.
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	// Setters
	void SetSkeleton(const class Skeleton* sk) {
		mSkeleton = sk;
	}

	// ��� ������ ���Ӱ� ������ �� ��� �ȷ�Ʈ ��� �Լ��� ȣ���ϰ�
	// �ִϸ��̼� ��� �ð��� ��ȯ�Ѵ�.
	float PlayAnimation(const class Animation* anim, float playRate = 1.f);

	TypeID GetType() const override { return TSkeletalMeshComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
protected:
	// ���� ���� ���� ��� * ���� �ι��� ���ε� ���� ��� ����� ��� �ȷ�Ʈ �׸��� ��ȯ
	void ComputeMatrixPalette();
	const class Skeleton* mSkeleton;

	// ��� �ȷ�Ʈ
	MatrixPalette mPalette;
	// ���� ����ǰ� �ִ� �ִϸ��̼�
	const class Animation* mAnimation;
	// �ִϸ��̼� ��� �ӵ�(1.0�� ���� �ӵ�)
	float mAnimPlayRate;
	// �ִϸ��̼��� ���� �ð�
	float mAnimTime;
};