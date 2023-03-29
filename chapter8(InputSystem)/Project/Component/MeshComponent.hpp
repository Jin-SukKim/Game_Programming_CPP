class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
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

protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
};