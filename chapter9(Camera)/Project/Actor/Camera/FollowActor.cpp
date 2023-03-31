#include "PreCompiled.hpp"

FollowActor::FollowActor(Game* game) : Actor(game)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
	SetPosition(Vector3d(0.f, 0.f, -100.f));

	mMoveComp = new MoveComponent(this);
	mCameraComp = new FollowCamera(this);
	// ī�޶� ��ġ �ʱ�ȭ
	mCameraComp->SnapToIdeal();
}

void FollowActor::ActorInput(const InputState& state)
{
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;

	if (state.Keyboard.GetKeyState(SDL_SCANCODE_W) == EHeld)
	{
		forwardSpeed += 400.f;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_S) == EHeld)
	{
		forwardSpeed -= 400.f;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_A) == EHeld)
	{
		angularSpeed -= MathUtils::PI;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_D) == EHeld)
	{
		angularSpeed += MathUtils::PI;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);

	// ���ǵ忡 ���� ���� �Ÿ� ����
	if (!MathUtils::NearZero(forwardSpeed))
	{
		mCameraComp->SetHorzDist(500.f);
	}
	else
	{
		mCameraComp->SetHorzDist(350.f);
	}
}

void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}