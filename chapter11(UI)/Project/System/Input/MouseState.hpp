// 마우스 입력
class MouseState
{
public:
	friend class InputSystem;

	// 마우스 위치
	const Vector2d& GetPosition() const {
		return mMousePos;
	}

	// 버튼
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;

	bool IsRelative() const {
		return mIsRelative;
	}
private:
	// 마우스 위치 저장
	Vector2d mMousePos;
	// 버튼 데이터 저장
	Uint32 mCurrButtons;
	Uint32 mPrevButtons;
	// 상대 마우스 모드 지원
	bool mIsRelative;
	// 마우스 휠
	Vector2d mScrollWheel;
};