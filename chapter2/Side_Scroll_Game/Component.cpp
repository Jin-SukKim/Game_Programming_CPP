#include "Component.hpp"

Component::Component(Actor* owner, int updateOrder) : mOwner(owner), mUpdateOrder(updateOrder) {
	// Actor의 컴포넌트 추가
	mOwner->AddComponent(this);
}

Component::~Component() {
	// Actor의 컴포넌트 삭제
	mOwner->RemoveComponent(this);
}