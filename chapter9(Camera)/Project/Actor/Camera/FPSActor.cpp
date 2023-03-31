#include "PreCompiled.hpp"

FPSActor::FPSActor(Game* game) : Actor(game)
{
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.f;
	mFootstep = mAudioComp->PlayEvent("event:/Footstep");
	mFootstep.SetPaused(true);

	mCameraComp = new FPSCamera(this);

	mFPSModel = new Actor(game);
	mFPSModel->SetScale(0.75f);
	mMeshComp = new MeshComponent(mFPSModel);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// 발소리
	mLastFootstep -= deltaTime;
	if (!MathUtils::NearZero(mMoveComp->GetForwardSpeed())
		&& mLastFootstep <= 0.f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}

	// Actor 위치에 대한 FPS 모델의 상대적인 위치 갱신
	const Vector3d modelOffset(Vector3d(10.f, 10.f, -10.f));
	Vector3d modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;
	modelPos.z += modelOffset.z;
	mFPSModel->SetPosition(modelPos);

	// Actor 회전값으로 회전값 초기화
	Quaternion q = GetRotation();
	// 카메라 pitch 값으로 회전
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), 
		mCameraComp->GetPitch()));
	mFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const struct InputState& state)
{
	float forwardSpeed = 0.f;
	float strafeSpeed = 0.f;

	// wasd 이동
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
		strafeSpeed -= 400.f;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_D) == EHeld)
	{
		strafeSpeed += 400.f;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);

	// 마우스 움직임

	// SDL로 부터 상대적인 이동을 얻는다.
	int x, y;
	Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);

	// 마우스 이동은 -500에서 500 사이의 값이라고 추정
	const int maxMouseSpeed = 500;
	// 초당 회전의 최대 속도
	const float maxAngularSpeed = MathUtils::PI * 8;

	float angularSpeed = 0.f;
	if (x != 0)
	{
		// [-1.0, 1.0] 범위로 변환
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		// 초당 회전을 곱한다.
		angularSpeed *= maxAngularSpeed;
	}

	mMoveComp->SetAngularSpeed(angularSpeed);

	// 피치 y 회전

	// 최대 회전 속도
	const float maxPitchSpeed = MathUtils::PI * 8;
	float pitchSpeed = 0.f;
	if (y != 0)
	{
		// 범위 제한 [-1.0, 1.0]
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::SetFootstepSurface(float value)
{
	// FMOD 파라미터 변경하면 자동으로 플레이 되므로 잠시 정지
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}