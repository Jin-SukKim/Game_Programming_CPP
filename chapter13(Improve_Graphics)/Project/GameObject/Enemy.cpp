#include "PreCompiled.hpp"

Enemy::Enemy(class Game* game) : Actor(game){
	// 적에 vector 추가
	game->GetEnemies().emplace_back(this);

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Airplane.png"));
	// 시작 타일 위치
	SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());
	// 내비게이션 설정
	NavComponent* nc = new NavComponent(this);
	nc->SetForwardSpeed(150.f);
	nc->StartPath(GetGame()->GetGrid()->GetStartTile());
	// 물리 충돌을 위한 원 설정
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(25.f);
}

Enemy::~Enemy() {
	// 적의 vector 삭제
	auto iter = std::find(GetGame()->GetEnemies().begin(),
		GetGame()->GetEnemies().end(), this);
	GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	// 끝 타일에 가까운지 확인
	Vector2d diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();
	if (MathUtils::NearZero(diff.Length(), 10.f)) {
		SetState(EDead);
	}
}