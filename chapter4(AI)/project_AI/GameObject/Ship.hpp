#pragma once
class Ship : public Actor {
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
private:
	SpriteComponent* sc;
	InputComponent* in;

	// 플레이어가 레이저를 너무 많이 만들지 못하는 제약
	float mLaserCoolDown;
};