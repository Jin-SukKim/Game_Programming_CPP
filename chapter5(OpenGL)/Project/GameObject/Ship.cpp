#include "..\PreCompiled.hpp"

Ship::Ship(class Game* game) : Actor(game), mLaserCoolDown(0.f) {
	// ���ּ� �ؽ�ó�� ���� �ִϸ��̼� ��������Ʈ ����
	sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	in = new InputComponent(this);
	// �Է�Ű ����
	in->SetForwardKey(SDL_SCANCODE_W);
	in->SetBackKey(SDL_SCANCODE_S);
	in->SetClockwiseKey(SDL_SCANCODE_A);
	in->SetCounterClockwiseKey(SDL_SCANCODE_D);
	// �ӵ�, ȸ�� ����
	in->SetMaxForwardSpeed(300.f);
	in->SetMaxAngularSpeed(MathUtils::TwoPI);
}

void Ship::UpdateActor(float deltaTime) {
	if (in->isMove()) {
		sc->SetTexture(GetGame()->GetTexture("Assets/ShipWithThrust.png"));
	}
	else {
		sc->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));
	}

	mLaserCoolDown -= deltaTime;
}

void Ship::ActorInput(const uint8_t* keyState) {
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCoolDown <= 0.f) {
		// Laser �����
		Laser* laser = new Laser(GetGame());
		// ���� ���� ��ġ���� ����
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// ������ �� Ÿ�� ����
		mLaserCoolDown = 0.5f;
	}
}