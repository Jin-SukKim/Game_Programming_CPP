#include "Actor.hpp"

class Component {
public:
	// ������
	
	// (������Ʈ �������� �������� ������Ʈ�� �� ���� ���ŵȴ�)
	Component(Actor* owner, int updateOrder = 100);
	// �Ҹ���
	virtual ~Component();
	// delta time���� ������Ʈ ������Ʈ
	virtual void Update(float deltaTime);

	int GetUpdateOrder() const {
		return mUpdateOrder;
	}

// ��Ӱ����� Ŭ�������� �����Ѵ�.
protected:
	// ������ actor
	// ������Ʈ�� �ʿ��ϴٰ� �ǴܵǴ� ��ȯ ������ �� ���� ������ �����ϱ� ���� Actor �����͸� ������.
	Actor* mOwner;

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