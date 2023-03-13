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
	void SetPosition(const Vector2d& pos) { mPosition = pos; }
	void SetState(State state) { mState = state; }
	void SetScale(float scale) { mScale = scale; }
	void SetRotation(float rotation) { mRotation = rotation; }

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
	Vector2d GetForward();
private:
	// Actor�� ����
	State mState;
	// ��ȯ
	Vector2d mPosition;	// Actor�� �߽���
	float mScale;		// Actor�� ����(100%�� ��� 1.f)
	float mRotation;	// ȸ�� ���� (Radian)

	// Actor�� ������ ������Ʈ��
	std::vector<class Component*> mComponents;
	class Game* mGame;

};