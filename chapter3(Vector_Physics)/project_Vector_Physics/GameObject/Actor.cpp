#include "..\PreCompiled.hpp"

// 의존성 주입(dependency injection) 접근법 사용
Actor::Actor(Game* game) : mState(EActive), mPosition(Vector2d::Zero),
mScale(1.f), mRotation(0.f), mGame(game) {
	mGame->AddActor(this);
}

Actor::~Actor() {
	// Actor를 Game에서 삭제
	mGame->RemoveActor(this);

	// 컴포넌트를 삭제해준다.
	while (!mComponents.empty()) {
		delete mComponents.back();
	}
}

// UpdateComponents를 먼저 호출한 후 UpdateActor를 호출한다.
void Actor::Update(float deltaTime) {
	// Actor가 EActive상태일때만 Actor를 갱신해준다.
	if (mState == EActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

// 모든 컴포넌트를 반복하며 순서대로 각 컴포넌트를 갱신
void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : mComponents) {
		comp->Update(deltaTime);
	}
}

void Actor::AddComponent(Component* component) {
	// 삽입 위치를 정렬된 vector에서 찾는다.
	// (첫 element가 더 크다)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	// iterator의 위치 전에 삽입해준다.
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
	// component를 찾는다.
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) {
		mComponents.erase(iter);
	}
}

// 각도에 따른 전방 벡터 구하기
Vector2d Actor::GetForward() {
	return Vector2d(MathUtils::Cos(mRotation), -MathUtils::Sin(mRotation));
}