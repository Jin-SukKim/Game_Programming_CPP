#pragma once
class Ship : public Actor {
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
private:
	SpriteComponent* sc;
	InputComponent* in;

	// �÷��̾ �������� �ʹ� ���� ������ ���ϴ� ����
	float mLaserCoolDown;
};