// 입력의 4가지 상태
enum ButtonState
{
	ENone,
	EPressed,
	EReleased,
	EHeld
}; 

// 키보드 입력

class KeyboardState
{
public:
	// InputSystem을 friend로 선언해 멤버 데이터를 쉽게 갱신하게 한다.
	friend class InputSystem;

	// 키의 이진값 true/false를 얻는다.
	bool GetKeyValue(SDL_Scancode keyCode) const;

	// 현재 이전 프레임을 토대로 상태를 얻는다.
	ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
	// 현재상태
	const Uint8* mCurrState;
	// 이전 프레임 상태(512사이즈)
	Uint8 mPrevState[SDL_NUM_SCANCODES];
};