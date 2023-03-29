#include "..\PreCompiled.hpp"

Asteroid::Asteroid(Game* game) : Actor(game), mCircle(nullptr) {
	// 랜덤하게 위치와 방향을 초기화
	Vector2d randPos = Random::GetVector(Vector2d(-512.0f, -384.0f),
		Vector2d(512.0f, 384.0f));
	SetPosition(randPos);

	SetRotation(Random::GetFloatRange(0.f, MathUtils::TwoPI));

	// 스프라이트 컴포넌트를 생성하고 텍스처를 설정
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));
	
	// 이동 컴포넌트 생성, 전방 속도 설정
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.f);

	// 운석을 게임에 추가하기
	game->AddAsteroid(this);

}

Asteroid::~Asteroid() {
	GetGame()->RemoveAsteroid(this);

}