#include "..\PreCompiled.hpp"

// ������ ����(dependency injection) ���ٹ� ���
Actor::Actor(Game* game) : mState(EActive), mPosition(Vector3d::Zero),
mScale(1.f), mRotation(Quaternion::Identity), 
mGame(game), mRecomputeWorldTransform(true) {
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
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		// Component ������Ʈ �� Actor�� ��ġ���� �ٲ��ٰ� ����
		ComputeWorldTransform();
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
Vector3d Actor::GetForward() const {
	// �ʱ� ���� ����(+x)�� ȸ�� ���ʹϾ����� ��ȯ�� ��´�.
	return Vector3d::Transform(Vector3d::UnitX, mRotation);
}

void Actor::ProcessInput(const uint8_t* keyState) {
	// Actor�� ������ Ȱ��ȭ�� Ȯ��
	if (mState == EActive) {
		// ��� ������Ʈ�� ProcessInput�� ȣ��
		for (auto comp : mComponents) {
			comp->ProcessInput(keyState);
		}
		// �����ǵ� ������ �ϵ��� ActorInput�� ȣ��
		ActorInput(keyState);
	}
}

// World Space�� transform ����
void Actor::ComputeWorldTransform() {
	if (mRecomputeWorldTransform) {
		mRecomputeWorldTransform = false;
		// ũ�� * ȸ�� * �̵�
		mWorldTransform = Matrix4x4::CreateScale(mScale);
		mWorldTransform *= Matrix4x4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4x4::CreateTranslation(mPosition);

		// Component���� world transform ������ �˸���
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}