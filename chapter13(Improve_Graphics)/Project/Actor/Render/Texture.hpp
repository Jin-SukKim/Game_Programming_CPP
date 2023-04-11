class Texture
{
public:
	Texture();
	~Texture();

	// �̹����κ��� �ؽ�ó ������Ʈ ����
	bool Load(const std::string& fileName);
	// �ؽ�ó ������Ʈ ����
	void Unload();

	// �ؽ�ó ������Ʈ Ȱ��ȭ
	void SetActive(int index = 0);

	// �ؽ�ó�� ����
	int GetWidth() const {
		return mWidth;
	}
	// �ؽ�ó�� ����
	int GetHeight() const {
		return mHeight;
	}

	unsigned int GetTextureID() const {
		return mTextureID;
	}

	// SDL_Surface�� Texture�� ��ȯ�Ѵ�.
	void CreateFromSurface(struct SDL_Surface* surface);

	// �ؽ�ó �������� ���� �ؽ�ó ����
	void CreateForRendering(int width, int height, unsigned int format);
private:
	// �ؽ�ó�� OpenGL ID
	unsigned int mTextureID;
	// �ؽ�ó�� ����/����
	int mWidth;
	int mHeight;
};