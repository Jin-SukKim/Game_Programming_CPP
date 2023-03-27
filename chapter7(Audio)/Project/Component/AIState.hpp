// �� ���¸� ��Ÿ���� Ŭ����

// �⺻ Ŭ������ ���¸� �����ϰ��� �� ���� ���� �Լ��� ����
class AIState {
public:
	AIState(class AIComponent* owner);

	// �� ������ ��ü���� �ൿ
	// ���� ���� �Լ��� ������ �ϴ��� ������ ���ǵ��� ������ �ݵ�� override�Ǿ���Ѵ�.

	// ���¸� ����
	virtual void Update(float deltaTime) = 0;
	// ���� ���� ���� ���� �ڵ带 ����
	virtual void OnEnter() = 0;
	// ���� ���� �ڵ带 ����
	virtual void OnExit() = 0;

	// ������ �̸� ���
	virtual const char* GetName() const = 0;
protected:
	// AIComponent�� ����
	class AIComponent* mOwner;
};


// AIState�� ����Ŭ������ AIPatrol Ŭ����(���� ��� ����)
class AIPatrol : public AIState {
public:
	AIPatrol(class AIComponent* owner);

	// �� ���¿� ���� ������ ������
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override {
		return "Patrol";
	}
};

// ���� ��� ����
class AIDeath : public AIState {
public:
	AIDeath(class AIComponent* owner);

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override {
		return "Death";
	}
};

// ���� ����
class AIAttack : public AIState {
public:
	AIAttack(class AIComponent* owner);

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override {
		return "Attack";
	}
};