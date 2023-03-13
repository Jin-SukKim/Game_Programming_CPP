#include "..\PreCompiled.hpp"

// ������ ����(dependency injection) ���ٹ� ���
Actor::Actor(Game* game) : mState(EActive), mPosition(Vector2d::Zero),
mScale(1.f), mRotation(0.f), mGame(game) {
	mGame->AddActor(this);
}

Actor::~Actor() {
	// Actor�� Game���� ����
	mGame->RemoveActor(this);

	// ������Ʈ�� �������ش�.
	while (!mComponents.empty()) {
		delete mComponents.back();
	}
}

// UpdateComponents�� ���� ȣ���� �� UpdateActor�� ȣ���Ѵ�.
void Actor::Update(float deltaTime) {
	// Actor�� EActive�����϶��� Actor�� �������ش�.
	if (mState == EActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

// ��� ������Ʈ�� �ݺ��ϸ� ������� �� ������Ʈ�� ����
void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : mComponents) {
		comp->Update(deltaTime);
	}
}

void Actor::AddComponent(Component* component) {
	// ���� ��ġ�� ���ĵ� vector���� ã�´�.
	// (ù element�� �� ũ��)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	// iterator�� ��ġ ���� �������ش�.
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
	// component�� ã�´�.
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) {
		mComponents.erase(iter);
	}
}

// ������ ���� ���� ���� ���ϱ�
Vector2d Actor::GetForward() {
	return Vector2d(MathUtils::Cos(mRotation), -MathUtils::Sin(mRotation));
}