#include "../PreCompiled.hpp"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder) 
	: Component(owner, updateOrder), mAngularSpeed(0.f), mForwardSpeed(0.f) {}

// Component Ŭ������ mOwner ��� ������ ���� �ڽ��� �����ڿ� ������ �� �ִ�.
// mOwner �����͸� ����� ������ Actor�� ��ġ��, ȸ����, ���� ���Ͱ��� ������ �� �ִ�.
void MoveComponent::Update(float deltaTime) {
	// �ӵ��� 0�� ������ ȸ���̳� ��ġ�� �������� �ʴ´�.
	if (!MathUtils::NearZero(mAngularSpeed))
	{
		Quaternion rot = mOwner->GetRotation();
		float angle = mAngularSpeed * deltaTime;
		// ȸ�� �������� ���� ���ʹϾ��� ����
		// �������� �������� ȸ��
		Quaternion inc(Vector3d::UnitZ, angle);
		// ���� ���ʹϾ�� �� ���ʹϾ��� �����Ѵ�.
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