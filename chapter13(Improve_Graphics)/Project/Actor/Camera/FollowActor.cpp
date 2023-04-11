#include "..\..\PreCompiled.hpp"

FollowActor::FollowActor(Game* game) : Actor(game), mMoving(false)
{
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/CatWarrior.gpmesh"));
	mMeshComp->SetSkeleton(game->GetSkeleton("Assets/CatWarrior.gpskel"));
	mMeshComp->PlayAnimation(game->GetAnimation("Assets/CatActionIdle.gpanim"));
	SetPosition(Vector3d(0.f, 0.f, -100.f));

	mMoveComp = new MoveComponent(this);
	mCameraComp = new FollowCamera(this);
	mCameraComp->SnapToIdeal();

	// ��̷� ī�޶� �߰�
	MirrorCamera* mirror = new MirrorCamera(this);
	mirror->SnapToIdeal();
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


	// ���ǵ忡 ���� ���� �Ÿ� ����
	if (!mMoving && !MathUtils::NearZero(forwardSpeed))
	{
		mMoving = true;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatRunSprint.gpanim"),
			1.25f);
		// ĳ���Ϳ� ī�޶� ������ �Ÿ� ����
		//mCameraComp->SetHorzDist(400.f);
	}
	else if (mMoving && MathUtils::NearZero(forwardSpeed))
	{
		mMoving = false;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatActionIdle.gpanim"));
		//mCameraComp->SetHorzDist(350.f);
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);
}

void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}