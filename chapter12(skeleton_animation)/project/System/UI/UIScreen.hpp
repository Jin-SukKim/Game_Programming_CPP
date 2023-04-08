class UIScreen
{
public:
	UIScreen(class Game* game);

	virtual ~UIScreen();
	// UIScreen의 서브클래스는 알 ㅐ함수들을 재정의할 수 있다.

	// UI 스크린의 상태를 갱신
	virtual void Update(float deltaTime);
	// 화면에 그린다.
	virtual void Draw(class Shader* shader);
	// 입력 처리
	virtual void ProcessInput(const struct InputState& state);
	virtual void HandleKeyPress(int key);

	// UI의 활성화 여부를 기록
	enum UIState { EActive, EClosing };
	// 상태를 closing으로 설정
	void Close();
	// UI 스크린의 상태를 얻는다.
	UIState GetState() const {
		return mState;
	}
	// 제목 텍스트를 변경
	void SetTitle(const std::string& text,
		const Vector3d& color = Vector3d::White,
		int pointSize = 40);

	// 새 버튼 추가
	void AddButton(const std::string& name, std::function<void()> onClick);
protected:
	// 텍스처를 그리기 위한 헬퍼 함수
	void DrawTexture(class Shader* shader, class Texture* texture,
		const Vector2d& offset = Vector2d::Zero,
		float scale = 1.f);
	class Game* mGame;
	// UI 스크린의 제목 텍스트 렌더링을 위한 정보
	class Font* mFont;
	class Texture* mTitle;
	Vector2d mTitlePos;
	// UI 상태
	UIState mState;

	class Texture* mBackground;
	Vector2d mBGPos;

	// 마우스 모드 상대적인지 아닌지 설정
	void SetRelativeMouseMode(bool relative);

	// 버튼 포인터 맵
	std::vector<class Button*> mButtons;
	class Texture* mButtonOn;
	class Texture* mButtonOff;
	Vector2d mNextButtonPos;

};