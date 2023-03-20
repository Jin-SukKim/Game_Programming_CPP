#include "..\PreCompiled.hpp"

NavComponent::NavComponent(Actor* owner, int updateOrder)
	: MoveComponent(owner) {}

/*
void NavComponent::Update(float deltaTime) {
	// 다음 지점에 도착했다면 이동할 새 지점을 얻고 해당 지점을 향해 회전한다.
	Vector2d diff = mOwner->GetPosition() - mNextPoint;
	if (diff.Length() <= 2.f) {
		mNextPoint = GetNextPoint();
		TurnTo(mNextPoint);
	}

	// Actor를 전진
	MoveComponent::Update(deltaTime);
}
*/
void NavComponent::Update(float deltaTime)
{

	MoveComponent::Update(deltaTime);
}

// Actor를 향하고자 하는 점으로 회전시킨다.
void NavComponent::TurnTo(const Vector2d& pos) {
	// 향하려는 점으로의 벡터를 구한다.
	Vector2d dir = pos - mOwner->GetPosition();
	// 새 각도는 이 방향 벡터의 atan2이다.
	// y값을 반전시킨다. +y는 화면 아래방향이기 때문이다.
	float angle = MathUtils::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}
