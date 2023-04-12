// 입력의 현재 상태를 포함하는 래퍼
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

	// SDL_PollEvents 루프 직전에 호출된다
	// 모든 '현재' 데이터를 이전 버퍼로 복사한다.
	void PrepareForUpdate();
	// SDL_PoolEvents 루프 직후에 호출된다.
	void Update();

	// 현재 입력 상태
	const InputState& GetState() const {
		return mState;
	}

	// 상대 마우스 모드 지원을 위한 활성화 함수
	void SetRelativeMouseMode(bool value);
	
	// SDL 이벤트
	void ProcessEvent(SDL_Event& event);

private:
	InputState mState;
	SDL_GameController* mController;

	// 1차원 축을 가진 스틱을 위한 필터링
	float Filter1D(int input);
	// 2차운 축을 가진 스틱을 취한 필터링
	Vector2d Filter2D(int inputx, int inputY);
};