class MoveComponent : public Component {
public:
	// ���� ������Ʈ �ǵ��� ���� �������� ����
	// ���� ������ Actor�� ������ ������Ʈ�� ������ �����Ѵ�.
	MoveComponent(class Actor* owner, int updateOrder = 10);

	// Actor�� �����̴� �Լ�
	void Update(float deltaTime) override;

	float GetAngularSpeed() const {
		return mAngularSpeed;
	}

	float GetForwardSpeed() const {
		return mForwardSpeed;
	}

	float GetStrafeSpeed() const {
		return mStrafeSpeed;
	}

	void SetAngularSpeed(float speed) {
		mAngularSpeed = speed;
	}
	void SetForwardSpeed(float speed) {
		mForwardSpeed = speed;
	}
	void SetStrafeSpeed(float speed) {
		mStrafeSpeed = speed;
	}

private:
	// ȸ������ ����(�ʴ� radian)
	float mAngularSpeed;
	// ���� �̵��� ����(�ʴ� ����)
	float mForwardSpeed;
	// �¿� �̵� �ӵ�
	float mStrafeSpeed;
};