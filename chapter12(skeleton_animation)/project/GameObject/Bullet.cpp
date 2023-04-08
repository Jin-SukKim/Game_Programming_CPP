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

	// ���� �浹�ϴ��� Ȯ��
	for (Enemy* e : GetGame()->GetEnemies()) {
		if (Intersect(*mCircle, *(e->GetCircle()))) {
			// �Ѿ˰� �� �Ѵ� ����
			e->SetState(EDead);
			SetState(EDead);
			break;
		}
	}

	mLiveTime -= deltaTime;
	if (mLiveTime <= 0.f) {
		// �Ѿ��� �ڿ��Ҹ�� �ð�
		SetState(EDead);
	}
}