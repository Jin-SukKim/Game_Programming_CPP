#include "../PreCompiled.hpp"

InputComponent::InputComponent(Actor* owner)
	: MoveComponent(owner), mForwardKey(0), mBackKey(0), 
	mClockwiseKey(0), mCounterClockwiseKey(0), isMoving(false)
{}

void InputComponent::ProcessInput(const uint8_t* keyState) {
	// MoveComponent�� ���� ���� �ӵ� ���
	float forwardSpeed = 0.f;
	SetIsMoving(false);
	// ������ Ű�� ���� �ùٸ� ���� �ӵ��� ����
	if (keyState[mForwardKey]) {
		forwardSpeed += mMaxForwardSpeed;
		SetIsMoving(true);
	}

	if (keyState[mBackKey]) {
		forwardSpeed -= mMaxForwardSpeed;
		SetIsMoving(true);
	}
	// �ӵ��� ����� �Լ��� SetForwardSpeed�� �ѱ��
	SetForwardSpeed(forwardSpeed);

	// �� �ӵ� ���
	float angularSpeed = 0.f;
	if (keyState[mClockwiseKey]) {
		angularSpeed += mMaxAngularSpeed;
		SetIsMoving(true);
	}

	if (keyState[mCounterClockwiseKey]) {
		angularSpeed -= mMaxAngularSpeed;
		SetIsMoving(true);
	}
	SetAngularSpeed(angularSpeed);
}