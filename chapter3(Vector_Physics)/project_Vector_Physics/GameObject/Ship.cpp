#include "..\PreCompiled.hpp"

Ship::Ship(class Game* game) : Actor(game), mGame(game) {
	// 우주선 텍스처를 가진 애니메이션 스프라이트 생성
	asc = new AnimSpriteComponent(this);
	anims = {
		game->GetTexture("Assets/Ship.png")
	};
	asc->SetAnimTextures(anims);

	in = new InputComponent(this);
	// 입력키 설정
	in->SetForwardKey(SDL_SCANCODE_W);
	in->SetBackKey(SDL_SCANCODE_S);
	in->SetClockwiseKey(SDL_SCANCODE_A);
	in->SetCounterClockwiseKey(SDL_SCANCODE_D);
	// 속도, 회전 설정
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