#include "PreCompiled.hpp"

Bullet::Bullet(class Game* game) : Actor(game) {
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Projectile.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(400.f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(5.f);

	mLiveTime = 1.f;
}


void Bullet::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	// 적과 충돌하는지 확인
	for (Enemy* e : GetGame()->GetEnemies()) {
		if (Intersect(*mCircle, *(e->GetCircle()))) {
			// 총알과 적 둘다 죽음
			e->SetState(EDead);
			SetState(EDead);
			break;
		}
	}

	mLiveTime -= deltaTime;
	if (mLiveTime <= 0.f) {
		// 총알이 자연소면될 시간
		SetState(EDead);
	}
}