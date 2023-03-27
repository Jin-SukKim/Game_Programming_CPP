#include "..\PreCompiled.hpp"

// 의존성 주입(dependency injection) 접근법 사용
Actor::Actor(Game* game) : mState(EActive), mPosition(Vector3d::Zero),
mScale(1.f), mRotation(Quaternion::Identity), 
mGame(game), mRecomputeWorldTransform(true) {
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
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		// Component 업데이트 후 Actor의 위치등이 바꼇다고 가정
		ComputeWorldTransform();
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
Vector3d Actor::GetForward() const {
	// 초기 전방 벡터(+x)를 회전 쿼터니언으로 변환해 얻는다.
	return Vector3d::Transform(Vector3d::UnitX, mRotation);
}

void Actor::ProcessInput(const uint8_t* keyState) {
	// Actor의 상태의 활성화를 확인
	if (mState == EActive) {
		// 모든 컴포넌트의 ProcessInput을 호출
		for (auto comp : mComponents) {
			comp->ProcessInput(keyState);
		}
		// 재정의된 행위를 하도록 ActorInput을 호출
		ActorInput(keyState);
	}
}

// World Space로 transform 적용
void Actor::ComputeWorldTransform() {
	if (mRecomputeWorldTransform) {
		mRecomputeWorldTransform = false;
		// 크기 * 회전 * 이동
		mWorldTransform = Matrix4x4::CreateScale(mScale);
		mWorldTransform *= Matrix4x4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4x4::CreateTranslation(mPosition);

		// Component한테 world transform 갱신을 알린다
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}