#include "..\PreCompiled.hpp"

const char* Actor::TypeNames[NUM_ACTOR_TYPES] = {
	"Actor",
	"BallActor",
	"FollowActor",
	"PlaneActor",
	"TargetActor",
};

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

// �¿� �̵�
Vector3d Actor::GetRight() const
{
	// ���ʹϾ� ȸ���� ����� ���� ���� ���� ȸ��
	return Vector3d::Transform(Vector3d::UnitY, mRotation);
}

void Actor::ProcessInput(const InputState& state) {
	// Actor�� ������ Ȱ��ȭ�� Ȯ��
	if (mState == EActive) {
		// ��� ������Ʈ�� ProcessInput�� ȣ��
		for (auto comp : mComponents) {
			comp->ProcessInput(state);
		}
		// �����ǵ� ������ �ϵ��� ActorInput�� ȣ��
		ActorInput(state);
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

void Actor::RotateToNewForward(const Vector3d& forward)
{
	// ���� ���Ϳ� ���� ������ ������ ���Ѵ�.
	float dot = Vector3d::Dot(Vector3d::UnitX, forward);
	float angle = MathUtils::Acos(dot);
	
	
	// ���� ������ +X���� Ȯ��
	if (dot > 0.9999f)
	{	
		SetRotation(Quaternion::Identity);
	}
	// -X ��������
	else if (dot < -0.9999f)
	{ 
		SetRotation(Quaternion(Vector3d::UnitZ, MathUtils::PI));
	}
	else
	{ 
		// ������ ���� ���� ���� �������� ȸ��
		Vector3d axis = Vector3d::Cross(Vector3d::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

void Actor::LoadProperties(const rapidjson::Value& inObj)
{
	// ���°��� ��� ���� ���ڿ� ���
	std::string state;
	if (JsonHelper::GetString(inObj, "state", state))
	{
		if (state == "active")
		{
			SetState(EActive);
		}
		else if (state == "paused")
		{
			SetState(EPaused);
		}
		else if (state == "dead")
		{
			SetState(EDead);
		}
	}

	// ��ġ, ȸ��, �����ϰ��� ��� ��ȯ ��� ���
	JsonHelper::GetVector3(inObj, "position", mPosition);
	JsonHelper::GetQuaternion(inObj, "rotation", mRotation);
	JsonHelper::GetFloat(inObj, "scale", mScale);
	ComputeWorldTransform(); 
	
}

void Actor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	std::string state = "active";
	if (mState == EPaused)
	{
		state = "paused";
	}
	else if (mState == EDead)
	{
		state = "dead";
	}

	JsonHelper::AddString(alloc, inObj, "state", state);
	JsonHelper::AddVector3(alloc, inObj, "position", mPosition);
	JsonHelper::AddQuaternion(alloc, inObj, "rotation", mRotation);
	JsonHelper::AddFloat(alloc, inObj, "scale", mScale);
}
