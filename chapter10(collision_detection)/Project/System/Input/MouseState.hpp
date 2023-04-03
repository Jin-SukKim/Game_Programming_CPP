// ���콺 �Է�
class MouseState
{
public:
	friend class InputSystem;

	// ���콺 ��ġ
	const Vector2d& GetPosition() const {
		return mMousePos;
	}

	// ��ư
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;

	bool IsRelative() const {
		return mIsRelative;
	}
private:
	// ���콺 ��ġ ����
	Vector2d mMousePos;
	// ��ư ������ ����
	Uint32 mCurrButtons;
	Uint32 mPrevButtons;
	// ��� ���콺 ��� ����
	bool mIsRelative;
	// ���콺 ��
	Vector2d mScrollWheel;
};