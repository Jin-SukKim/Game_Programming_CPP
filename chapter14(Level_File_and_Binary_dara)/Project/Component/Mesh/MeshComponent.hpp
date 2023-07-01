class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner, bool isSkeletal = false);
	~MeshComponent();

	// ������ ���̴��� �� �޽� ������Ʈ�� �׸���.
	virtual void Draw(class Shader* shader);
	// �޽� ������Ʈ�� ����ϴ� �޽� ����
	virtual void SetMesh(class Mesh* mesh)
	{
		mMesh = mesh;
	}
	// �ؽ�ó ����
	void SetTextureIndex(size_t index)
	{
		mTextureIndex = index;
	}

	void SetVisible(bool visible) {
		mVisible = visible;
	}
	bool GetVisible() const {
		return mVisible;
	}

	bool GetIsSkeletal() const {
		return mIsSkeletal;
	}

	TypeID GetType() const override { return TMeshComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
	bool mVisible;
	bool mIsSkeletal;
};