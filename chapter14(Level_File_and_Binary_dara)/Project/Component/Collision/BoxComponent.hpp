class BoxComponent : public Component
{
public:
	BoxComponent(class Actor* owner, int updateOrder = 100);
	~BoxComponent();

	void OnUpdateWorldTransform() override;

	void SetObjectBox(const AABB& model) {
		mObjectBox = model;
	}
	const AABB& GetWorldBox() const {
		return mWorldBox;
	}
	void SetShouldRotate(bool value) {
		mShouldRotate = value;
	}


	TypeID GetType() const override { return TBoxComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

private:
	AABB mObjectBox;
	AABB mWorldBox;
	bool mShouldRotate;
};