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
	void SetActive();

	// �ؽ�ó�� ����
	int GetWidth() const {
		return mWidth;
	}
	// �ؽ�ó�� ����
	int GetHeight() const {
		return mHeight;
	}

private:
	// �ؽ�ó�� OpenGL ID
	unsigned int mTextureID;
	// �ؽ�ó�� ����/����
	int mWidth;
	int mHeight;
};