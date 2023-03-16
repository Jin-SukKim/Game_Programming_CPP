#include "PreCompiled.hpp"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder) 
	: Component(owner, updateOrder), mAngularSpeed(0), mForwardSpeed(0) {}

// Component Ŭ������ mOwner ��� ������ ���� �ڽ��� �����ڿ� ������ �� �ִ�.
// mOwner �����͸� ����� ������ Actor�� ��ġ��, ȸ����, ���� ���Ͱ��� ������ �� �ִ�.
void MoveComponent::Update(float deltaTime) {
	// �ӵ��� 0�� ������ ȸ���̳� ��ġ�� �������� �ʴ´�.
	if (!MathUtils::NearZero(mAngularSpeed)) {
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	if (!MathUtils::NearZero(mForwardSpeed)) {
		Vector2d pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

		// ������Ʈ�� ȭ�� ������ ������ ȭ�� ������ �Ű��ֱ�
		if (pos.x < 0.0f) { pos.x = 1022.0f; }
		else if (pos.x > 1024.0f) { pos.x = 2.0f; }

		if (pos.y < 0.0f) { pos.y = 766.0f; }
		else if (pos.y > 768.0f) { pos.y = 2.0f; }

		mOwner->SetPosition(pos);
	}
}