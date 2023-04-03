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

	// �ڽ� ������Ʈ �߰�
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3d(-25.f, -25.f, -87.5f),
		Vector3d(25.f, 25.f, 87.5f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// BoxComponent�� ��찪�� ����
	FixCollisions();

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

void FPSActor::Shoot()
{
	// ������ (ȭ�� �߾�)
	Vector3d screenPoint(0.f, 0.f, 0.f);
	Vector3d start = GetGame()->GetRenderer()->Unproject(screenPoint);
	// ������ ���� (ȭ�� �߾��̳� ���� �� ���)
	screenPoint.z = 0.9f;
	Vector3d end = GetGame()->GetRenderer()->Unproject(screenPoint);
	// ���� ����
	Vector3d dir = end - start;
	dir.Normalize();
	// �� ����
	BallActor* ball = new BallActor(GetGame());
	ball->SetPlayer(this);
	ball->SetPosition(start + dir * 20.f);
	// ���ο� �������� �� ȸ��
	ball->RotateToNewForward(dir);
	// ���� �Ҹ�
	mAudioComp->PlayEvent("event:/Shot");
}

void FPSActor::FixCollisions()
{
	// �ڽ��� �����ϱ� ���� �ڽ��� ���� ��ȯ ������ �ʿ�
	ComputeWorldTransform();

	const AABB& playerBox = mBoxComp->GetWorldBox();
	Vector3d pos = GetPosition();

	auto& planes = GetGame()->GetPlanes();
	for (auto pa : planes)
	{
		// PlaneActor�� �浹�ߴ��� �׽�Ʈ
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			// �� ������ ��ħ���� ���
			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			// dx1/dx2 ���� ������ ���� ���� ���� dx�� ����
			float dx = MathUtils::Abs(dx1) < MathUtils::Abs(dx2) ? dx1 : dx2;
			float dy = MathUtils::Abs(dy1) < MathUtils::Abs(dy2) ? dy1 : dy2;
			float dz = MathUtils::Abs(dz1) < MathUtils::Abs(dz2) ? dz1 : dz2;

			// �ּ� ��ħ�� ���� ���� ������ x/y/z ��ġ�� ����
			// dx�� ���� ���� ���
			if (MathUtils::Abs(dx) <= MathUtils::Abs(dy) &&
				MathUtils::Abs(dx) <= MathUtils::Abs(dz))
			{
				pos.x += dx;
			}
			// dy�� ���� ���� ���
			else if (MathUtils::Abs(dy) <= MathUtils::Abs(dx) &&
				MathUtils::Abs(dy) <= MathUtils::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// ����� ��ġ�� �����ϰ� �ڽ� ������Ʈ�� ����
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}
	}
}