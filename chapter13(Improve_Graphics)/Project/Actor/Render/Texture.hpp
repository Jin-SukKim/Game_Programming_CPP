class Texture
{
public:
	Texture();
	~Texture();

	// 이미지로부터 텍스처 오브젝트 생성
	bool Load(const std::string& fileName);
	// 텍스처 오브젝트 삭제
	void Unload();

	// 텍스처 오브젝트 활성화
	void SetActive(int index = 0);

	// 텍스처의 넓이
	int GetWidth() const {
		return mWidth;
	}
	// 텍스처의 높이
	int GetHeight() const {
		return mHeight;
	}

	unsigned int GetTextureID() const {
		return mTextureID;
	}

	// SDL_Surface를 Texture로 변환한다.
	void CreateFromSurface(struct SDL_Surface* surface);

	// 텍스처 렌더링을 위한 텍스처 생성
	void CreateForRendering(int width, int height, unsigned int format);
private:
	// 텍스처의 OpenGL ID
	unsigned int mTextureID;
	// 텍스처의 넓이/높이
	int mWidth;
	int mHeight;
};