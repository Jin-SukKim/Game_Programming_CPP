// �Է��� 4���� ����
enum ButtonState
{
	ENone,
	EPressed,
	EReleased,
	EHeld
}; 

// Ű���� �Է�

class KeyboardState
{
public:
	// InputSystem�� friend�� ������ ��� �����͸� ���� �����ϰ� �Ѵ�.
	friend class InputSystem;

	// Ű�� ������ true/false�� ��´�.
	bool GetKeyValue(SDL_Scancode keyCode) const;

	// ���� ���� �������� ���� ���¸� ��´�.
	ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
	// �������
	const Uint8* mCurrState;
	// ���� ������ ����(512������)
	Uint8 mPrevState[SDL_NUM_SCANCODES];
};