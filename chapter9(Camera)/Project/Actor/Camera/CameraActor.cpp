#include "..\PreCompiled.hpp"

CameraActor::CameraActor(Game* game)
	:Actor(game)
{
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.f;
	mFootstep = mAudioComp->PlayEvent("event:/Footstep");
	mFootstep.SetPaused(true);
}

void CameraActor::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	// 발소리 사운드 시작
	mLastFootstep -= deltaTime;
	if (!MathUtils::NearZero(mMoveComp->GetForwardSpeed())
		&& mLastFootstep <= 0.f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}

	// 카메라 위치
	Vector3d cameraPos = GetPosition();
	// 카메라 앞 점 위치
	Vector3d target = GetPosition() + GetForward() * 100.f;
	// 카메라 위 방향(뒤집혔는지 아닌지 확인할 수 있게)
	Vector3d up = Vector3d::UnitZ;

	// 뷰 행렬
	Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);

	// 오디오를 위해 리스너 설정
	GetGame()->GetAudioSystem()->SetListener(view);
}

void CameraActor::ActorInput(const InputState& state)
{
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;

	// wasd로 움직이기
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_W) == EHeld)
	{
		forwardSpeed += 300.0f;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_S) == EHeld)
	{
		forwardSpeed -= 300.0f;
	}
	// 회전
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_A) == EHeld)
	{
		angularSpeed -= MathUtils::TwoPI;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_D) == EHeld)
	{
		angularSpeed += MathUtils::TwoPI;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);

}

void CameraActor::SetFootstepSurface(float value)
{
	// 여기서 멈춘다.
	// 파라미터 값이 변경되면 발소리를 재생해준다.
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}
