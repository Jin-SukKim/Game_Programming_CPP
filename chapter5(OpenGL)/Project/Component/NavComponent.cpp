#include "..\PreCompiled.hpp"

NavComponent::NavComponent(Actor* owner, int updateOrder)
	: MoveComponent(owner) {}

/*
void NavComponent::Update(float deltaTime) {
	// ���� ������ �����ߴٸ� �̵��� �� ������ ��� �ش� ������ ���� ȸ���Ѵ�.
	Vector2d diff = mOwner->GetPosition() - mNextPoint;
	if (diff.Length() <= 2.f) {
		mNextPoint = GetNextPoint();
		TurnTo(mNextPoint);
	}

	// Actor�� ����
	MoveComponent::Update(deltaTime);
}
*/
void NavComponent::Update(float deltaTime)
{

	MoveComponent::Update(deltaTime);
}

// Actor�� ���ϰ��� �ϴ� ������ ȸ����Ų��.
void NavComponent::TurnTo(const Vector2d& pos) {
	// ���Ϸ��� �������� ���͸� ���Ѵ�.
	Vector2d dir = pos - mOwner->GetPosition();
	// �� ������ �� ���� ������ atan2�̴�.
	// y���� ������Ų��. +y�� ȭ�� �Ʒ������̱� �����̴�.
	float angle = MathUtils::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}
