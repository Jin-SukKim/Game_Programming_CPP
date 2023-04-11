class Font
{
public:
	Font(class Game* game);
	~Font();
	// 파일로부터 폰트를 로드 및 언로드
	bool Load(const std::string& fileName);
	void Unload();
	// 해당 폰트와 문자열로 텍스처에 그린다.
	// 폰트 크기를 사용해 주어진 문자열 텍스처를 생성한다.
	class Texture* RenderText(const std::string& text,
		const Vector3d& color = Vector3d::White,
		int pointSize = 30);

private:
	// 폰트 크기와 폰트 데이터를 매핑
	std::unordered_map<int, TTF_Font*> mFontData;
	class Game* mGame;
};