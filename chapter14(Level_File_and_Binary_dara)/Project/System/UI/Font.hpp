class Font
{
public:
	Font(class Game* game);
	~Font();
	// ���Ϸκ��� ��Ʈ�� �ε� �� ��ε�
	bool Load(const std::string& fileName);
	void Unload();
	// �ش� ��Ʈ�� ���ڿ��� �ؽ�ó�� �׸���.
	// ��Ʈ ũ�⸦ ����� �־��� ���ڿ� �ؽ�ó�� �����Ѵ�.
	class Texture* RenderText(const std::string& text,
		const Vector3d& color = Vector3d::White,
		int pointSize = 30);

private:
	// ��Ʈ ũ��� ��Ʈ �����͸� ����
	std::unordered_map<int, TTF_Font*> mFontData;
	class Game* mGame;
};