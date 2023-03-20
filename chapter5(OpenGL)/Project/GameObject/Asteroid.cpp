#include "..\PreCompiled.hpp"

Asteroid::Asteroid(Game* game) : Actor(game), mCircle(nullptr) {
	// �����ϰ� ��ġ�� ������ �ʱ�ȭ
	Vector2d randPos = Random::GetVector(Vector2d(-512.0f, -384.0f),
		Vector2d(512.0f, 384.0f));
	SetPosition(randPos);

	SetRotation(Random::GetFloatRange(0.f, MathUtils::TwoPI));

	// ��������Ʈ ������Ʈ�� �����ϰ� �ؽ�ó�� ����
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));
	
	// �̵� ������Ʈ ����, ���� �ӵ� ����
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.f);

	// ��� ���ӿ� �߰��ϱ�
	game->AddAsteroid(this);

}

Asteroid::~Asteroid() {
	GetGame()->RemoveAsteroid(this);

}