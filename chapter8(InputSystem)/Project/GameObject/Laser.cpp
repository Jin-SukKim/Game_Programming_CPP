#include "..\PreCompiled.hpp"

Laser::Laser(Game* game) : Actor(game), mDeathTimer(1.f) {
	// 스프라이트 component 생성
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));
	
	// 이동 컴포넌트 생성
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.f);
}

void Laser::UpdateActor(float deltaTime) {
	// 일정 시간이 지나면 레이저를 없애준다
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.f) {
		SetState(EDead);
	}
	else {
		// 교차 테스트
		for (auto ast : GetGame()->GetAsteroids()) {
			if (Intersect(*mCircle, *(ast->GetCircle()))) {
				// 첫 교차하는 운석과 레이저를 없애준다.
				SetState(EDead);
				ast->SetState(EDead);
				break;
			}
		}
	}
}
