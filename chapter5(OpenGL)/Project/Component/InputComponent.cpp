#include "../PreCompiled.hpp"

InputComponent::InputComponent(Actor* owner)
	: MoveComponent(owner), mForwardKey(0), mBackKey(0), 
	mClockwiseKey(0), mCounterClockwiseKey(0), isMoving(false)
{}

void InputComponent::ProcessInput(const uint8_t* keyState) {
	// MoveComponent를 위한 전방 속도 계산
	float forwardSpeed = 0.f;
	SetIsMoving(false);
	// 눌러진 키에 따라 올바른 전방 속도를 결점
	if (keyState[mForwardKey]) {
		forwardSpeed += mMaxForwardSpeed;
		SetIsMoving(true);
	}

	if (keyState[mBackKey]) {
		forwardSpeed -= mMaxForwardSpeed;
		SetIsMoving(true);
	}
	// 속도를 상속한 함수인 SetForwardSpeed로 넘긴다
	SetForwardSpeed(forwardSpeed);

	// 각 속도 계싼
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