class UIScreen
{
public:
	UIScreen(class Game* game);

	virtual ~UIScreen();
	// UIScreen�� ����Ŭ������ �� ���Լ����� �������� �� �ִ�.

	// UI ��ũ���� ���¸� ����
	virtual void Update(float deltaTime);
	// ȭ�鿡 �׸���.
	virtual void Draw(class Shader* shader);
	// �Է� ó��
	virtual void ProcessInput(const struct InputState& state);
	virtual void HandleKeyPress(int key);

	// UI�� Ȱ��ȭ ���θ� ���
	enum UIState { EActive, EClosing };
	// ���¸� closing���� ����
	void Close();
	// UI ��ũ���� ���¸� ��´�.
	UIState GetState() const {
		return mState;
	}
	// ���� �ؽ�Ʈ�� ����
	void SetTitle(const std::string& text,
		const Vector3d& color = Vector3d::White,
		int pointSize = 40);

	// �� ��ư �߰�
	void AddButton(const std::string& name, std::function<void()> onClick);
protected:
	// �ؽ�ó�� �׸��� ���� ���� �Լ�
	void DrawTexture(class Shader* shader, class Texture* texture,
		const Vector2d& offset = Vector2d::Zero,
		float scale = 1.f);
	class Game* mGame;
	// UI ��ũ���� ���� �ؽ�Ʈ �������� ���� ����
	class Font* mFont;
	class Texture* mTitle;
	Vector2d mTitlePos;
	// UI ����
	UIState mState;

	class Texture* mBackground;
	Vector2d mBGPos;

	// ���콺 ��� ��������� �ƴ��� ����
	void SetRelativeMouseMode(bool relative);

	// ��ư ������ ��
	std::vector<class Button*> mButtons;
	class Texture* mButtonOn;
	class Texture* mButtonOff;
	Vector2d mNextButtonPos;

};