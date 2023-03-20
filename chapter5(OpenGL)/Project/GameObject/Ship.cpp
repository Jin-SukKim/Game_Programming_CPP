#include "..\PreCompiled.hpp"

Ship::Ship(class Game* game) : Actor(game), mLaserCoolDown(0.f) {
	// 우주선 텍스처를 가진 애니메이션 스프라이트 생성
	sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

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
		sc->SetTexture(GetGame()->GetTexture("Assets/ShipWithThrust.png"));
	}
	else {
		sc->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));
	}

	mLaserCoolDown -= deltaTime;
}

void Ship::ActorInput(const uint8_t* keyState) {
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCoolDown <= 0.f) {
		// Laser 만들기
		Laser* laser = new Laser(GetGame());
		// 현재 배의 위치에서 시작
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// 레이저 쿨 타임 리셋
		mLaserCoolDown = 0.5f;
	}
}