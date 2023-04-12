class PointLightComponent : public Component
{
public:
	PointLightComponent(class Actor* owner);
	~PointLightComponent();

	// 점광의 기하를 그린다.
	void Draw(class Shader* shader, class Mesh* mesh);

	// 분산 색상
	Vector3d mDiffuseColor;
	// 광원의 반지름
	float mInnerRadius;
	float mOuterRadius;

	TypeID GetType() const override { return TPointLightComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
};