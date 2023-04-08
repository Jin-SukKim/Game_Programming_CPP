#include "PreCompiled.hpp"

Tower::Tower(class Game* game) : Actor(game) {
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));

	mMove = new MoveComponent(this);

	mNextAttack = AttackTime;
}

void Tower::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	mNextAttack -= deltaTime;
	if (mNextAttack <= 0.f) {
		Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
		if (e != nullptr) {
			// 적의 벡터
			Vector2d dir = e->GetPosition() - GetPosition();
			// 적과 타워의 거리
			float dist = dir.Length();
			if (dist < AttackRange) {
				// 적 바라보기
				SetRotation(MathUtils::Atan2(-dir.y, dir.x));
				// 공격
				Bullet* b = new Bullet(GetGame());
				b->SetPosition(GetPosition());
				b->SetRotation(GetRotation());
			}
		}
		mNextAttack += AttackTime;
	}
}