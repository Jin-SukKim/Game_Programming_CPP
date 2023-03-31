class Mesh 
{
public:
	Mesh();
	~Mesh();

	// 메시 로드
	bool Load(const std::string& fileName, class Renderer* renderer);
	// 메시 언로드
	void UnLoad();

	// 이 메시에 해당하는 vertex 배열을 얻는다.
	class VertexArray* GetVertexArray() {
		return mVertexArray;
	}

	// 즉정 index에 해당하는 텍스처를 얻는다.
	class Texture* GetTexture(size_t index);

	// shader의 이름을 얻는다.
	const std::string& GetShaderName() const {
		return mShaderName;
	}

	// 오브젝트 공간 바운딩 구체 반지름을 얻는다.
	// 오브젝트 공간의 원점과 가장 먼 점과의 거리
	float GetRadius() const {
		return mRadius;
	}

	float GetSpecPower() const {
		return mSpecPower;
	}

private:
	// gpmesh 파일에서 지정된 각 텍스처에 해당
	// 이 메시에 사용되는 텍스처들
	std::vector<class Texture*> mTextures;
	// 메시의 vertex 배열, vertex/index buffer용
	class VertexArray* mVertexArray;
	// 메시가 지정한 shader 이름
	std::string mShaderName;
	// 오브젝트 공간 바운딩 구체의 반지름값 저장
	float mRadius;

	// 정반사 값
	float mSpecPower;
};