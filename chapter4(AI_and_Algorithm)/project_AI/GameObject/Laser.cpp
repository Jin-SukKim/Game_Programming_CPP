#include "PreCompiled.hpp"

Laser::Laser(Game* game) : Actor(game), mDeathTimer(1.f) {
	// ��������Ʈ component ����
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));
	
	// �̵� ������Ʈ ����
	MoveComponent* mc = new MoveComponent(this);
	mc->SetFoarwardSpeed(800.f);

	mCircle = new CircleComponent(this);
	mCircle->SetRarius(11.f);
}

void Laser::UpdateActor(float deltaTime) {
	// ���� �ð��� ������ �������� �����ش�
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.f) {
		SetState(EDead);
	}
	else {
		// ���� �׽�Ʈ
		for (auto ast : GetGame()->GetAsteroid()) {
			if (Intersect(*mCircle, *(ast->GetCircle()))) {
				// ù �����ϴ� ��� �������� �����ش�.
				SetState(EDead);
				ast->SetState(EDead);
				break;
			}
		}
	}
}
