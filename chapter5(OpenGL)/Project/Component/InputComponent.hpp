// ������ Actor�� �յ� �̵��� ȸ���� �����ϴ� Ư�� Ű�� �����ϴ� Ŭ����
class InputComponent : public MoveComponent {
public:
	InputComponent(class Actor* owner);

	// ���� MoveComponent�� ���� �ӵ��� �� �ӵ��� ����
	// ���������� �ӵ����� �������� �ʵ��� �ִ� �ӵ��� ����
	void ProcessInput(const uint8_t* keyState) override;

	// private ������ �����ϱ� ���� Getter/Setter
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
	// �ִ� ���� �ӵ�
	float mMaxForwardSpeed;
	// �ִ� �� �ӵ�
	float mMaxAngularSpeed;
	//���� Ű
	int mForwardKey;
	// ���� Ű
	int mBackKey;
	// �ð���� ȸ�� Ű
	int mClockwiseKey;
	// �ݽð���� ȸ�� Ű
	int mCounterClockwiseKey;

	bool isMoving;
};