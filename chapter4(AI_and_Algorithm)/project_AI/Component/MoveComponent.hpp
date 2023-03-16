class MoveComponent : public Component {
public:
	// 먼저 업데이트 되도록 갱신 순서값을 낮춤
	// 갱신 순서는 Actor가 갱신할 컴포넌트위 순서를 결정한다.
	MoveComponent(class Actor* owner, int updateOrder = 10);

	// Actor를 움직이는 함수
	void Update(float deltaTime) override;

	float GetAngularSpeed() const {
		return mAngularSpeed;
	}

	float GetForwardSpeed() const {
		return mForwardSpeed;
	}

	void SetAngularSpeed(float speed) {
		mAngularSpeed = speed;
	}
	void SetFoarwardSpeed(float speed) {
		mForwardSpeed = speed;
	}

private:
	// 회전ㅇ르 제어(초당 radian)
	float mAngularSpeed;
	// 전방 이동을 제어(초당 단위)
	float mForwardSpeed;
};