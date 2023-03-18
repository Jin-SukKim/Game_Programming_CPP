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
			// ���� ����
			Vector2d dir = e->GetPosition() - GetPosition();
			// ���� Ÿ���� �Ÿ�
			float dist = dir.Length();
			if (dist < AttackRange) {
				// �� �ٶ󺸱�
				SetRotation(MathUtils::Atan2(-dir.y, dir.x));
				// ����
				Bullet* b = new Bullet(GetGame());
				b->SetPosition(GetPosition());
				b->SetRotation(GetRotation());
			}
		}
		mNextAttack += AttackTime;
	}
}