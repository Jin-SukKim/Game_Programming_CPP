#include "../PreCompiled.hpp"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder) 
	: Component(owner, updateOrder), mAngularSpeed(0.f), mForwardSpeed(0.f) {}

// Component Ŭ������ mOwner ��� ������ ���� �ڽ��� �����ڿ� ������ �� �ִ�.
// mOwner �����͸� ����� ������ Actor�� ��ġ��, ȸ����, ���� ���Ͱ��� ������ �� �ִ�.
void MoveComponent::Update(float deltaTime) {
	// �ӵ��� 0�� ������ ȸ���̳� ��ġ�� �������� �ʴ´�.
	if (!MathUtils::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	if (!MathUtils::NearZero(mForwardSpeed))
	{
		Vector2d pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

		// Screen wrapping (for asteroids)
		if (pos.x < -512.0f) { pos.x = 510.0f; }
		else if (pos.x > 512.0f) { pos.x = -510.0f; }
		if (pos.y < -384.0f) { pos.y = 382.0f; }
		else if (pos.y > 384.0f) { pos.y = -382.0f; }
		mOwner->SetPosition(pos);
	}
}