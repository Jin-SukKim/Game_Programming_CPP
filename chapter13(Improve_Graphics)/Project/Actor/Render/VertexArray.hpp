class VertexArray {
public:
	// �ٸ� Ÿ���� vertex layout ����
	enum Layout
	{
		PosNormTex,		// �� ���� �ؽ�ó
		PosNormSkinTex	// �� ���� �ؽ�ó
	};

	// ������ �ε��� ���� �����͸� ���ڷ� �޾� �����͸� OpenGL�� ����
	// (�ش� �����ʹ� �׷��� �ϵ����� �ε�ȴ�)
	VertexArray(const void* verts, unsigned int numVerts, Layout layout,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// ���� �迭�� Ȱ��ȭ (Ȱ��ȭ�� �ϸ� ���� �迩�� �׸� ���� �ִ�)
	void SetActive();

	unsigned int GetNumIndices() const {
		return mNumIndices;
	}
	unsigned int GetNumVerts() const {
		return mNumVerts;
	}

private:
	// OpenGL�� ������ ��ü�� �����͸� ��ȯ���� �ʾ� private���� ����
	// ��� ������ ID�� �����ش�.
	// ���� ���ۿ� �����ϴ� ������ ��
	unsigned int mNumVerts;
	// �ε��� ���ۿ� �����ϴ� �ε����� ��
	unsigned int mNumIndices;
	// ���� ������ OpneGL ID
	unsigned int mVertexBuffer;
	// �ε��� ������ OpenGL ID
	unsigned int mIndexBuffer;
	// ���� �迭 ��ü�� openGL ID
	unsigned int mVertexArray;
};