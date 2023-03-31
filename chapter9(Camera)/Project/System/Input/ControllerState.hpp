class ControllerState
{
public:
	friend class InputSystem;

	// ��ư
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;

	bool GetIsConnected() const {
		return mIsConnected;
	}

	// ���� Ʈ���� ��
	float GetLeftTrigger() const {
		return mLeftTrigger;
	}
	// ������ Ʈ���� ��
	float GetRightTrigger() const {
		return mRightTrigger;
	}


private:
	// ���� ��ư
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];
	// ��Ʈ�ѷ� ���� ����
	bool mIsConnected;

	// ����, ������ Ʈ����
	float mLeftTrigger;
	float mRightTrigger;

	// �Ƴ��α� ��ƽ
	Vector2d mLeftStick;
	Vector2d mRightStick;

};