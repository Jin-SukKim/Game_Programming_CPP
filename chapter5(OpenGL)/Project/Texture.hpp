class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	void SetActive();

	int GetWidth() const {
		return mWidth;
	}
	int GetHeight() const {
		return mHeight;
	}

private:
	// 텍스처의 OpenGL ID
	unsigned int mTextureID;
	// 텍스처의 넓이/높이
	int mWidth;
	int mHeight;
};