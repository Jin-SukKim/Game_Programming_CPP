#include "Component.hpp"

Component::Component(Actor* owner, int updateOrder) : mOwner(owner), mUpdateOrder(updateOrder) {
	// Actor�� ������Ʈ �߰�
	mOwner->AddComponent(this);
}

Component::~Component() {
	// Actor�� ������Ʈ ����
	mOwner->RemoveComponent(this);
}