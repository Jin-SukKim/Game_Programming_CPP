#include "../PreCompiled.hpp"

InputComponent::InputComponent(Actor* owner)
	: MoveComponent(owner), mForwardKey(0), mBackKey(0), 
	mClockwiseKey(0), mCounterClockwiseKey(0), isMoving(false)
{}

void InputComponent::ProcessInput(const InputState& state) {
	// MoveComponent�� ���� ���� �ӵ� ���
	float forwardSpeed = 0.f;
	SetIsMoving(false);
	// ������ Ű�� ���� �ùٸ� ���� �ӵ��� ����
	if (state.Keyboard.GetKeyValue(SDL_Scancode(mForwardKey))) {
		forwardSpeed += mMaxForwardSpeed;
		SetIsMoving(true);
	}

	if (state.Keyboard.GetKeyValue(SDL_Scancode(mBackKey))) {
		forwardSpeed -= mMaxForwardSpeed;
		SetIsMoving(true);
	}
	// �ӵ��� ����� �Լ��� SetForwardSpeed�� �ѱ��
	SetForwardSpeed(forwardSpeed);

	// �� �ӵ� ���
	float angularSpeed = 0.f;
	if (state.Keyboard.GetKeyValue(SDL_Scancode(mClockwiseKey))) {
		angularSpeed += mMaxAngularSpeed;
		SetIsMoving(true);
	}

	if (state.Keyboard.GetKeyValue(SDL_Scancode(mCounterClockwiseKey))) {
		angularSpeed -= mMaxAngularSpeed;
		SetIsMoving(true);
	}
	SetAngularSpeed(angularSpeed);
}