#pragma once

// ���� ��ü ���� �⺻ Ŭ����
class Actor {
public:
	enum TypeID
	{
		TActor = 0,
		TBallActor,
		TFollowActor,
		TPlaneActor,
		TTargetActor,

		NUM_ACTOR_TYPES
	};

	static const char* TypeNames[NUM_ACTOR_TYPES];

	// Actor�� ���¸� �����ϴµ� ���
	// EActive�϶��� ����, EDead���´� ���ӿ��� �����϶�� ���� ��
	enum State { EActive, EPaused, EDead };

	// ������
	Actor(class Game* game);
	// �Ҹ���
	virtual ~Actor();

	// Setters
	// Actor�� ũ��, ȸ��, �̵��� ���� setting�� ������
	// world space���� ��ȭ�� �����Ƿ� worldtransform�� true�� ����
	void SetPosition(const Vector3d& pos) { 
		mPosition = pos; 
		mRecomputeWorldTransform = true;
	}
	void SetState(State state) { mState = state; }
	void SetScale(float scale) { 
		mScale = scale; 
		mRecomputeWorldTransform = true;
	}
	void SetRotation(const Quaternion& rotation) { 
		mRotation = rotation; 
		mRecomputeWorldTransform = true;
	}
	void SetRotation(float rotation) { 
		mRotation2 = rotation;  
		mRecomputeWorldTransform = true; 
	}
	float GetRotation2() const { return mRotation2; }


	// Getters
	State GetState() const { return mState; }
	const Vector3d GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	Quaternion GetRotation() const { return mRotation; }

	class Game* GetGame() { return mGame; }

	// Game���� ȣ���ϴ� Update �Լ�
	void Update(float deltaTime);
	// Actor�� ������ ��� ������Ʈ�� ������Ʈ
	void UpdateComponents(float deltaTime);
	// Ư�� Actor�� Ưȭ�� ������Ʈ �ڵ�
	// �⺻ ������ ��������� ����Ŭ������ �Լ��� �������� ���氡��
	virtual void UpdateActor(float deltaTime) {}

	// ������Ʈ �߰�/����
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// ���� ���� - �ʱ� ���� ����(+x)�� ȸ�� ���ʹϾ����� ��ȯ�� ��´�.
	Vector3d GetForward() const;
	// �¿� �̵�
	Vector3d GetRight() const;

	// Game�� ȣ���ϴ� ProcessInput�Լ�
	void ProcessInput(const struct InputState& state);
	// Ư�� Actor�� ���� �Է� �ڵ�
	virtual void ActorInput(const struct InputState& state) {};

	// ���� ��ȯ ���(model space -> world space)
	void ComputeWorldTransform();
	const Matrix4x4& GetWorldTransform() const {
		return mWorldTransform;
	}

	// ȸ������ �����ϴ� �Լ�
	void RotateToNewForward(const Vector3d& forward);

	// ���Ϳ� ���� �Ӽ� �ε�
	virtual void LoadProperties(const rapidjson::Value& inObj);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const;

	// �ùٸ� Ÿ���� ���͸� ����(���� Ÿ�Կ� ���� �߰� �Ķ���͸� �ε��ϱ� ����)
	template <typename T>
	static Actor* Create(class Game* game, const rapidjson::Value& inObj)
	{
		// Ÿ�� T�� ���͸� �������� �Ҵ�
		T* t = new T(game);
		// ���ο� ������ LoadProperties ȣ��
		t->LoadProperties(inObj);
		return t;
	}

	// mComponent ���͸� ��ȸ�ϰ� Ÿ�԰� ��ġ�ϴ� ������ ������Ʈ�� �����Ѵ�.
	Component* GetComponentOfType(Component::TypeID type)
	{
		Component* comp = nullptr;
		for (Component* c : mComponents)
		{
			if (c->GetType() == type)
			{
				comp = c;
				break;
			}
		}
		return comp;
	}

	virtual TypeID GetType() const { return TActor; }
	const std::vector<Component*>& GetComponents() const { return mComponents; }
private:
	// Actor�� ����
	State mState;

	// Actor�� ������ ������Ʈ��
	std::vector<class Component*> mComponents;
	class Game* mGame;

	// transform
	Matrix4x4 mWorldTransform;
	Vector3d mPosition;	// Actor�� �߽���
	float mScale;		// Actor�� ����(100%�� ��� 1.f)
	Quaternion mRotation;	// ȸ�� ���� (Radian)
	bool mRecomputeWorldTransform;

	float mRotation2;

};