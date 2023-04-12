#pragma once

// ���� ��ü ���� �⺻ Ŭ����
class Actor {
public:
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
	void SetPosition(const Vector2d& pos) { 
		mPosition = pos; 
		mRecomputeWorldTransform = true;
	}
	void SetState(State state) { mState = state; }
	void SetScale(float scale) { 
		mScale = scale; 
		mRecomputeWorldTransform = true;
	}
	void SetRotation(float rotation) { 
		mRotation = rotation; 
		mRecomputeWorldTransform = true;
	}

	// Getters
	State GetState() const { return mState; }
	const Vector2d GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	float GetRotation() const { return mRotation; }

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

	// ���� ����
	Vector2d GetForward() const;

	// Game�� ȣ���ϴ� ProcessInput�Լ�
	void ProcessInput(const uint8_t* keyState);
	// Ư�� Actor�� ���� �Է� �ڵ�
	virtual void ActorInput(const uint8_t* keyState) {};

	// ���� ��ȯ ���(model space -> world space)
	void ComputeWorldTransform();
	const Matrix4x4& GetWorldTransform() const {
		return mWorldTransform;
	}

private:
	// Actor�� ����
	State mState;

	// Actor�� ������ ������Ʈ��
	std::vector<class Component*> mComponents;
	class Game* mGame;

	// transform
	Matrix4x4 mWorldTransform;
	Vector2d mPosition;	// Actor�� �߽���
	float mScale;		// Actor�� ����(100%�� ��� 1.f)
	float mRotation;	// ȸ�� ���� (Radian)
	bool mRecomputeWorldTransform;

};