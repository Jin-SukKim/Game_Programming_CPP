#include "../PreCompiled.hpp"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder) 
	: Component(owner, updateOrder), mAngularSpeed(0.f), mForwardSpeed(0.f) {}

// Component 클래스가 mOwner 멤버 변수를 통해 자신의 소유자에 접근할 수 있다.
// mOwner 포인터를 사용해 소유자 Actor의 위치값, 회전값, 전방 벡터값에 접근할 수 있다.
void MoveComponent::Update(float deltaTime) {
	// 속도가 0에 가까우면 회전이나 위치를 갱신하지 않는다.
	if (!MathUtils::NearZero(mAngularSpeed))
	{
		Quaternion rot = mOwner->GetRotation();
		float angle = mAngularSpeed * deltaTime;
		// 회전 증가분을 위한 쿼터니언을 생성
		// 상향축을 기준으로 회전
		Quaternion inc(Vector3d::UnitZ, angle);
		// 이전 쿼터니언과 새 쿼터니언을 연결한다.
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}

	if (!MathUtils::NearZero(mForwardSpeed))
	{
		Vector3d pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}