class ControllerState
{
public:
	friend class InputSystem;

	// 버튼
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;

	bool GetIsConnected() const {
		return mIsConnected;
	}

	// 왼쪽 트리거 값
	float GetLeftTrigger() const {
		return mLeftTrigger;
	}
	// 오른쪽 트리거 값
	float GetRightTrigger() const {
		return mRightTrigger;
	}


private:
	// 현재 버튼
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];
	// 컨트롤러 연결 상태
	bool mIsConnected;

	// 왼쪽, 오른쪽 트리거
	float mLeftTrigger;
	float mRightTrigger;

	// 아날로그 스틱
	Vector2d mLeftStick;
	Vector2d mRightStick;

};