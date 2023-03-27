// 각 상태를 나타내는 클래스

// 기본 클래스로 상태를 제어하고자 몇 가지 가상 함수를 포함
class AIState {
public:
	AIState(class AIComponent* owner);

	// 각 상태의 구체적인 행동
	// 순수 가상 함수로 무엇을 하는지 당장은 정의되지 않으나 반드시 override되어야한다.

	// 상태를 갱신
	virtual void Update(float deltaTime) = 0;
	// 상태 진입 시의 전이 코드를 구형
	virtual void OnEnter() = 0;
	// 종료 전이 코드를 구현
	virtual void OnExit() = 0;

	// 상태의 이름 얻기
	virtual const char* GetName() const = 0;
protected:
	// AIComponent와 연결
	class AIComponent* mOwner;
};


// AIState의 서브클래스의 AIPatrol 클래스(순찰 기계 상태)
class AIPatrol : public AIState {
public:
	AIPatrol(class AIComponent* owner);

	// 이 상태에 대한 행위를 재정의
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override {
		return "Patrol";
	}
};

// 죽음 기계 상태
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

// 공격 상태
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