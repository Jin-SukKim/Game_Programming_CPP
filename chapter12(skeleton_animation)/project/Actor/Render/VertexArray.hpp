class VertexArray {
public:
	// 다른 타입의 vertex layout 지원
	enum Layout
	{
		PosNormTex,		// 본 없는 텍스처
		PosNormSkinTex	// 본 지원 텍스처
	};

	// 정점과 인데스 버퍼 포인터를 인자로 받아 데이터를 OpenGL로 전달
	// (해당 데이터는 그래픽 하드웨어상에 로드된다)
	VertexArray(const void* verts, unsigned int numVerts, Layout layout,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// 정점 배열을 활성화 (활성화를 하면 정점 배여을 그릴 수가 있다)
	void SetActive();

	unsigned int GetNumIndices() const {
		return mNumIndices;
	}
	unsigned int GetNumVerts() const {
		return mNumVerts;
	}

private:
	// OpenGL은 생성한 개체의 포인터를 반환하지 않아 private으로 선언
	// 대신 정수형 ID를 돌려준다.
	// 정점 버퍼에 존재하는 정점의 수
	unsigned int mNumVerts;
	// 인데스 버퍼에 존재하는 인덱스의 수
	unsigned int mNumIndices;
	// 정점 버퍼의 OpneGL ID
	unsigned int mVertexBuffer;
	// 인덱스 버퍼의 OpenGL ID
	unsigned int mIndexBuffer;
	// 정점 배열 개체의 openGL ID
	unsigned int mVertexArray;
};