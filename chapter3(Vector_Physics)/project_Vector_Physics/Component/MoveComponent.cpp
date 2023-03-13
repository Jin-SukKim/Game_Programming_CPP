#include "PreCompiled.hpp"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder) 
	: Component(owner, updateOrder), mAngularSpeed(0), mForwardSpeed(0) {}

// Component 클래스가 mOwner 멤버 변수를 통해 자신의 소유자에 접근할 수 있다.
// mOwner 포인터를 사용해 소유자 Actor의 위치값, 회전값, 전방 벡터값에 접근할 수 있다.
void MoveComponent::Update(float deltaTime) {
	// 속도가 0에 가까우면 회전이나 위치를 갱신하지 않는다.
	if (!MathUtils::NearZero(mAngularSpeed)) {
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	if (!MathUtils::NearZero(mForwardSpeed)) {
		Vector2d pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

		// 오브젝트가 화면 밖으로 나가면 화면 안으로 옮겨주기
		if (pos.x < 0.0f) { pos.x = 1022.0f; }
		else if (pos.x > 1024.0f) { pos.x = 2.0f; }

		if (pos.y < 0.0f) { pos.y = 766.0f; }
		else if (pos.y > 768.0f) { pos.y = 2.0f; }

		mOwner->SetPosition(pos);
	}
}