#include "PreCompiled.hpp"

Enemy::Enemy(class Game* game) : Actor(game){
	// ���� vector �߰�
	game->GetEnemies().emplace_back(this);

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Airplane.png"));
	// ���� Ÿ�� ��ġ
	SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());
	// ������̼� ����
	NavComponent* nc = new NavComponent(this);
	nc->SetForwardSpeed(150.f);
	nc->StartPath(GetGame()->GetGrid()->GetStartTile());
	// ���� �浹�� ���� �� ����
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(25.f);
}

Enemy::~Enemy() {
	// ���� vector ����
	auto iter = std::find(GetGame()->GetEnemies().begin(),
		GetGame()->GetEnemies().end(), this);
	GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	// �� Ÿ�Ͽ� ������� Ȯ��
	Vector2d diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();
	if (MathUtils::NearZero(diff.Length(), 10.f)) {
		SetState(EDead);
	}
}