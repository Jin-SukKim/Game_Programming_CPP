class GBuffer
{
public:
	// G 버퍼에 저장되는 여러 타입의 데이터
	enum Type
	{
		EDiffuse = 0,
		ENormal,
		EWorldPos,
		NUM_GBUFFER_TEXTURES
	};

	GBuffer();
	~GBuffer();

	// G 버퍼 생성 및 삭제
	bool Create(int width, int height);
	void Destroy();

	// 특정 데이터 타입의 텍스처를 얻는다.
	class Texture* GetTexture(Type type);
	// 프레임 버퍼 개체 ID를 얻는다.
	unsigned int GetBufferID() const {
		return mBufferID;
	}
	// 샘플링을 위한 모든 G 버퍼 텍스처를 활성화
	void SetTexturesActive();
private:
	// G 버퍼와 연관된 텍스처들
	std::vector<class Texture*> mTextures;
	// 프레임 버퍼 개체 ID
	unsigned int mBufferID;
};