#pragma once
#include "Actor.hpp"
#include "AnimSpriteComponent.hpp"

class Ship : public Actor {
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);

	// ����/������ �ӵ�
	float GetRightSpeed() const {
		return mRightSpeed;
	}
	// ��/�Ʒ� �ӵ�
	float GetDownSpeed() const {
		return mDownSpeed;
	}
private:
	float mRightSpeed;
	float mDownSpeed;
};