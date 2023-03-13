#pragma once
class Ship : public Actor {
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);

	// 왼쪽/오른쪽 속도
	float GetRightSpeed() const {
		return mRightSpeed;
	}
	// 위/아래 속도
	float GetDownSpeed() const {
		return mDownSpeed;
	}
private:
	float mRightSpeed;
	float mDownSpeed;
};