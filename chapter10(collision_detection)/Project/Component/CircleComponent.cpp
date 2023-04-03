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
	// 거리 제곱값 계산
	Vector3d diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();

	// 반지름 합의 제곱값 계산
	float radiusSq = a.GetRadius() + b.GetRadius();
	radiusSq *= radiusSq;

	// 두 중심 사이의 거리 제곱값과 반지름의 합의 제곱값을 비교한다.
	return distSq <= radiusSq;
}