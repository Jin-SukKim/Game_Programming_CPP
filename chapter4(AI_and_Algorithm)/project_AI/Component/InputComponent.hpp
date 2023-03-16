// 소유자 Actor의 앞뒤 이동과 회전을 제어하는 특정 키를 설정하는 클래스
class InputComponent : public MoveComponent {
public:
	InputComponent(class Actor* owner);

	// 직접 MoveComponent의 전방 속도와 각 속도를 설정
	// 비정상적인 속도값이 설정되지 않도록 최대 속도를 제한
	void ProcessInput(const uint8_t* keyState) override;

	// private 변수에 접근하기 위한 Getter/Setter
	void SetMaxForwardSpeed(float speed) {
		mMaxForwardSpeed = speed;
	}
	void SetMaxAngularSpeed(float speed) {
		mMaxAngularSpeed = speed;
	}
	void SetForwardKey(int key) {
		mForwardKey = key;
	}
	void SetBackKey(int key) {
		mBackKey = key;
	}
	void SetClockwiseKey(int key) {
		mClockwiseKey = key;
	}
	void SetCounterClockwiseKey(int key) {
		mCounterClockwiseKey = key;
	}

	float GetMaxForwardSpeed() {
		return mMaxForwardSpeed;
	}
	float GetMaxAngularSpeed() {
		return mMaxAngularSpeed;
	}
	int GetForwardKey() {
		return mForwardKey;
	}
	int GetBackKey() {
		return mBackKey;
	}
	int GetClockwiseKey() {
		return mClockwiseKey;
	}
	int GetCounterClockwiseKey() {
		return mCounterClockwiseKey;
	}

	void SetIsMoving(bool moving) {
		isMoving = moving;
	}

	bool isMove() {
		return isMoving;
	}
private:
	// 최대 전방 속도
	float mMaxForwardSpeed;
	// 최대 각 속도
	float mMaxAngularSpeed;
	//전진 키
	int mForwardKey;
	// 후진 키
	int mBackKey;
	// 시계방향 회전 키
	int mClockwiseKey;
	// 반시계방향 회전 키
	int mCounterClockwiseKey;

	bool isMoving;
};