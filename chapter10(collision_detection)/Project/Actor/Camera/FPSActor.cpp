#include "..\..\PreCompiled.hpp"

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

	// 박스 컴포넌트 추가
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3d(-25.f, -25.f, -87.5f),
		Vector3d(25.f, 25.f, 87.5f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// BoxComponent의 경곗값을 재계산
	FixCollisions();

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

void FPSActor::Shoot()
{
	// 시작점 (화면 중앙)
	Vector3d screenPoint(0.f, 0.f, 0.f);
	Vector3d start = GetGame()->GetRenderer()->Unproject(screenPoint);
	// 마지막 지점 (화면 중앙이나 제일 먼 평면)
	screenPoint.z = 0.9f;
	Vector3d end = GetGame()->GetRenderer()->Unproject(screenPoint);
	// 방향 벡터
	Vector3d dir = end - start;
	dir.Normalize();
	// 공 생성
	BallActor* ball = new BallActor(GetGame());
	ball->SetPlayer(this);
	ball->SetPosition(start + dir * 20.f);
	// 새로운 방향으로 공 회전
	ball->RotateToNewForward(dir);
	// 슈팅 소리
	mAudioComp->PlayEvent("event:/Shot");
}

void FPSActor::FixCollisions()
{
	// 박스를 갱신하기 저에 자신의 세계 변환 재계산이 필요
	ComputeWorldTransform();

	const AABB& playerBox = mBoxComp->GetWorldBox();
	Vector3d pos = GetPosition();

	auto& planes = GetGame()->GetPlanes();
	for (auto pa : planes)
	{
		// PlaneActor와 충돌했는지 테스트
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			// 각 축으로 겹침값을 계산
			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			// dx1/dx2 값중 절댓값이 가장 작은 값을 dx로 설정
			float dx = MathUtils::Abs(dx1) < MathUtils::Abs(dx2) ? dx1 : dx2;
			float dy = MathUtils::Abs(dy1) < MathUtils::Abs(dy2) ? dy1 : dy2;
			float dz = MathUtils::Abs(dz1) < MathUtils::Abs(dz2) ? dz1 : dz2;

			// 최소 겹침이 가장 작은 값으로 x/y/z 위치를 보정
			// dx가 가장 작을 경우
			if (MathUtils::Abs(dx) <= MathUtils::Abs(dy) &&
				MathUtils::Abs(dx) <= MathUtils::Abs(dz))
			{
				pos.x += dx;
			}
			// dy가 가장 작을 경우
			else if (MathUtils::Abs(dy) <= MathUtils::Abs(dx) &&
				MathUtils::Abs(dy) <= MathUtils::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// 재계산된 위치를 저장하고 박스 컴포넌트를 갱신
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}
	}
}