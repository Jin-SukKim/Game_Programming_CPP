#pragma once
class Ship : public Actor {
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const struct InputState& state) override;
private:
	Vector2d mVelocityDir;
	Vector2d mRotationDir;
	float mSpeed;
	float mLaserCooldown;
};