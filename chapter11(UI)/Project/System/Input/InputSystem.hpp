// �Է��� ���� ���¸� �����ϴ� ����
struct InputState
{
	KeyboardState Keyboard;
	MouseState Mouse;
	ControllerState Controller;
};


class InputSystem
{
public:
	bool Initialize();
	void Shutdown();

	// SDL_PollEvents ���� ������ ȣ��ȴ�
	// ��� '����' �����͸� ���� ���۷� �����Ѵ�.
	void PrepareForUpdate();
	// SDL_PoolEvents ���� ���Ŀ� ȣ��ȴ�.
	void Update();

	// ���� �Է� ����
	const InputState& GetState() const {
		return mState;
	}

	// ��� ���콺 ��� ������ ���� Ȱ��ȭ �Լ�
	void SetRelativeMouseMode(bool value);
	
	// SDL �̺�Ʈ
	void ProcessEvent(SDL_Event& event);

private:
	InputState mState;
	SDL_GameController* mController;

	// 1���� ���� ���� ��ƽ�� ���� ���͸�
	float Filter1D(int input);
	// 2���� ���� ���� ��ƽ�� ���� ���͸�
	Vector2d Filter2D(int inputx, int inputY);
};