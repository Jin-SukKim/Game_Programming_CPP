#pragma once
#include <vector>
#include "Game.hpp"
#include "Component.hpp"
#include "Vector2.hpp"
using namespace Math;

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
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	void SetState(State state) { mState = state; }
	void SetScale(float scale) { mScale = scale; }
	void SetRotation(float rotation) { mRotation = rotation; }

	// Getters
	State GetState() const { return mState; }
	const Vector2 GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	float GetRotation() const {	return mRotation; }
	Game* GetGame() { return mGame; }

	// Game���� ȣ���ϴ� Update �Լ�
	void Update(float deltaTime);
	// Actor�� ������ ��� ������Ʈ�� ������Ʈ
	void UpdateComponents(float deltaTime);
	// Ư�� Actor�� Ưȭ�� ������Ʈ �ڵ�
	// �⺻ ������ ��������� ����Ŭ������ �Լ��� �������� ���氡��
	virtual void UpdateActor(float deltaTime) {}

	// ������Ʈ �߰�/����
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
private:
	// Actor�� ����
	State mState;
	// ��ȯ
	Vector2 mPosition;	// Actor�� �߽���
	float mScale;		// Actor�� ����(100%�� ��� 1.f)
	float mRotation;	// ȸ�� ���� (Radian)

	// Actor�� ������ ������Ʈ��
	std::vector<Component*> mComponents;
	Game* mGame;
	
};