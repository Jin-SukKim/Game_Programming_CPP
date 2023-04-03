#include "..\PreCompiled.hpp"

CircleComponent::CircleComponent(Actor* owner)
	: Component(owner), mRadius(0.f) {}

const Vector3d CircleComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

float CircleComponent::GetRadius() const {
	return mOwner->GetScale() * mRadius;
}

bool Intersect(const CircleComponent& a, const CircleComponent& b) {
	// �Ÿ� ������ ���
	Vector3d diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();

	// ������ ���� ������ ���
	float radiusSq = a.GetRadius() + b.GetRadius();
	radiusSq *= radiusSq;

	// �� �߽� ������ �Ÿ� �������� �������� ���� �������� ���Ѵ�.
	return distSq <= radiusSq;
}