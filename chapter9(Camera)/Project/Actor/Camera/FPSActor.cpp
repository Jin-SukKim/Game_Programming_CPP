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

	// �߼Ҹ�
	mLastFootstep -= deltaTime;
	if (!MathUtils::NearZero(mMoveComp->GetForwardSpeed())
		&& mLastFootstep <= 0.f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}

	// Actor ��ġ�� ���� FPS ���� ������� ��ġ ����
	const Vector3d modelOffset(Vector3d(10.f, 10.f, -10.f));
	Vector3d modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;
	modelPos.z += modelOffset.z;
	mFPSModel->SetPosition(modelPos);

	// Actor ȸ�������� ȸ���� �ʱ�ȭ
	Quaternion q = GetRotation();
	// ī�޶� pitch ������ ȸ��
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), 
		mCameraComp->GetPitch()));
	mFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const struct InputState& state)
{
	float forwardSpeed = 0.f;
	float strafeSpeed = 0.f;

	// wasd �̵�
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

	// ���콺 ������

	// SDL�� ���� ������� �̵��� ��´�.
	int x, y;
	Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);

	// ���콺 �̵��� -500���� 500 ������ ���̶�� ����
	const int maxMouseSpeed = 500;
	// �ʴ� ȸ���� �ִ� �ӵ�
	const float maxAngularSpeed = MathUtils::PI * 8;

	float angularSpeed = 0.f;
	if (x != 0)
	{
		// [-1.0, 1.0] ������ ��ȯ
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		// �ʴ� ȸ���� ���Ѵ�.
		angularSpeed *= maxAngularSpeed;
	}

	mMoveComp->SetAngularSpeed(angularSpeed);

	// ��ġ y ȸ��

	// �ִ� ȸ�� �ӵ�
	const float maxPitchSpeed = MathUtils::PI * 8;
	float pitchSpeed = 0.f;
	if (y != 0)
	{
		// ���� ���� [-1.0, 1.0]
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::SetFootstepSurface(float value)
{
	// FMOD �Ķ���� �����ϸ� �ڵ����� �÷��� �ǹǷ� ��� ����
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}