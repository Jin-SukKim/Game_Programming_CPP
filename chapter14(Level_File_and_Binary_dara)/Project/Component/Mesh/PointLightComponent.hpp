class PointLightComponent : public Component
{
public:
	PointLightComponent(class Actor* owner);
	~PointLightComponent();

	// ������ ���ϸ� �׸���.
	void Draw(class Shader* shader, class Mesh* mesh);

	// �л� ����
	Vector3d mDiffuseColor;
	// ������ ������
	float mInnerRadius;
	float mOuterRadius;

	TypeID GetType() const override { return TPointLightComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
};