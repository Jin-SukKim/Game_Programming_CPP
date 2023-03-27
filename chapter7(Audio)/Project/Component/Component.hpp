#pragma once
class Component {
public:
	// ������
	// (������Ʈ �������� �������� ������Ʈ�� �� ���� ���ŵȴ�)
	Component(class Actor* owner, int updateOrder = 100);
	// �Ҹ���
	virtual ~Component();
	// delta time���� ������Ʈ ������Ʈ
	virtual void Update(float deltaTime) {}

	int GetUpdateOrder() const {
		return mUpdateOrder;
	}

	virtual void ProcessInput(const uint8_t* keyState) {};

	// �������� ���� ��ȯ�� ���ŵ� �� ������Ʈ���� �����Ѵ�.
	virtual void OnUpdateWorldTransform() {};

// ��Ӱ����� Ŭ�������� �����Ѵ�.
protected:
	// ������ actor
	// ������Ʈ�� �ʿ��ϴٰ� �ǴܵǴ� ��ȯ ������ �� ���� ������ �����ϱ� ���� Actor �����͸� ������.
	class Actor* mOwner;

	// ������Ʈ ������Ʈ ����
	/*
		���� ������ �����ϸ� �ſ� �����ϴ�.
		ex) 1��Ī ī�޶� ������Ʈ�� ��� �̵� ������Ʈ��
		�÷��̾ �̵���Ų ���� ���ŵǾ���Ѵ�.
		�� ���� ������ ���� Actor�� AddComponent�Լ��� ������Ʈ �߰� ��
		������Ʈ ���͸� �����Ѵ�.
	*/
	int mUpdateOrder;

};