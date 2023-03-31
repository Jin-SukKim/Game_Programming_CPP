#include "PreCompiled.hpp"

OrbitActor::OrbitActor(Game* game) : Actor(game)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
	SetPosition(Vector3d(0.f, 0.f, -100.f));

	mCameraComp = new OrbitCamera(this);
}

void OrbitActor::ActorInput(const InputState& state)
{
	// 마우스 회전
	int x, y;
	// 상대 마우스
	SDL_GetRelativeMouseState(&x, &y);

	// 우클릭 했을 때만 회전
	if (state.Mouse.GetButtonState(SDL_BUTTON_RIGHT) == EHeld)
	{
		// 마우스 움직임이 -500~500이라 가정
		const int maxMouseSpeed = 500;
		// 최대 속도일 때 rotation/sec
		const float maxOrbitSpeed = MathUtils::PI * 8;

		// 요 계산
		float yawSpeed = 0.f;
		if (x != 0)
		{
			// [-1.0, 1.0] 사이
			yawSpeed = static_cast<float>(x) / maxMouseSpeed;
			// rotation/sec 곱하기
			yawSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetYawSpeed(yawSpeed);

		// 피치 계산
		float pitchSpeed = 0.f;
		if (y != 0)
		{
			// [-1.0, 1.0] 사이
			pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
			// rotation/sec 곱하기
			pitchSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetPitchSpeed(pitchSpeed);
	}
}

void OrbitActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}