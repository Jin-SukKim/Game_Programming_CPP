class Mesh 
{
public:
	Mesh();
	~Mesh();

	// �޽� �ε�
	bool Load(const std::string& fileName, class Renderer* renderer);
	// �޽� ��ε�
	void UnLoad();

	// �� �޽ÿ� �ش��ϴ� vertex �迭�� ��´�.
	class VertexArray* GetVertexArray() {
		return mVertexArray;
	}

	// ���� index�� �ش��ϴ� �ؽ�ó�� ��´�.
	class Texture* GetTexture(size_t index);

	// shader�� �̸��� ��´�.
	const std::string& GetShaderName() const {
		return mShaderName;
	}

	// ������Ʈ ���� �ٿ�� ��ü �������� ��´�.
	// ������Ʈ ������ ������ ���� �� ������ �Ÿ�
	float GetRadius() const {
		return mRadius;
	}

	float GetSpecPower() const {
		return mSpecPower;
	}

	// AABB ����
	const AABB& GetBox() const {
		return mBox;
	}

	const std::string& GetFileName() const { return mFileName; }

	// Save the mesh in binary format
	void SaveBinary(const std::string& fileName, const void* verts,
		uint32_t numVerts, VertexArray::Layout layout,
		const uint32_t* indices, uint32_t numIndices,
		const std::vector<std::string>& textureNames,
		const AABB& box, float radius,
		float specPower);
	// Load in the mesh from binary format
	bool LoadBinary(const std::string& fileName, class Renderer* renderer);

private:
	// gpmesh ���Ͽ��� ������ �� �ؽ�ó�� �ش�
	// �� �޽ÿ� ���Ǵ� �ؽ�ó��
	std::vector<class Texture*> mTextures;
	// �޽��� vertex �迭, vertex/index buffer��
	class VertexArray* mVertexArray;
	// �޽ð� ������ shader �̸�
	std::string mShaderName;
	// ������Ʈ ���� �ٿ�� ��ü�� �������� ����
	float mRadius;

	// ���ݻ� ��
	float mSpecPower;

	// AABB
	AABB mBox;

	std::string mFileName;
};