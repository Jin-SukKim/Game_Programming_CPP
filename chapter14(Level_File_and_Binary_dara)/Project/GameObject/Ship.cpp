#include "..\PreCompiled.hpp"

Ship::Ship(class Game* game) : Actor(game), mSpeed(400.0f), mLaserCooldown(0.0f) {
	// 우주선 텍스처를 가진 애니메이션 스프라이트 생성
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));
}

void Ship::UpdateActor(float deltaTime) {

	mLaserCooldown -= deltaTime;

	// Update position based on velocity
	Vector2d pos = GetPosition();
	pos += mVelocityDir * mSpeed * deltaTime;
	SetPosition(pos);

	// Update rotation
	float angle = MathUtils::Atan2(mRotationDir.y, mRotationDir.x);
	SetRotation(angle);
}

void Ship::ActorInput(const uint8_t* keyState) {
	if (state.Controller.GetRightTrigger() > 0.25f
		&& mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (quarter second)
		mLaserCooldown = 0.25f;
	}

	if (state.Controller.GetIsConnected())
	{
		mVelocityDir = state.Controller.GetLeftStick();
		if (!Math::NearZero(state.Controller.GetRightStick().Length()))
		{
			mRotationDir = state.Controller.GetRightStick();
		}
	}
}