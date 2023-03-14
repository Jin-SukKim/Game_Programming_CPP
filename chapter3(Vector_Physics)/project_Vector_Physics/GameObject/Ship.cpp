#include "..\PreCompiled.hpp"

Ship::Ship(class Game* game) : Actor(game), mGame(game) {
	// ���ּ� �ؽ�ó�� ���� �ִϸ��̼� ��������Ʈ ����
	asc = new AnimSpriteComponent(this);
	anims = {
		game->GetTexture("Assets/Ship.png")
	};
	asc->SetAnimTextures(anims);

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
		anims[0] = mGame->GetTexture("Assets/ShipWithThrust.png");
		asc->SetAnimTextures(anims);
	}
	else {
		anims[0] = mGame->GetTexture("Assets/Ship.png");
		asc->SetAnimTextures(anims);
	}
}