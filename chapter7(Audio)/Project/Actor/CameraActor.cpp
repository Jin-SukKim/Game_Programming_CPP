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

	// �߼Ҹ� ���� ����
	mLastFootstep -= deltaTime;
	if (!MathUtils::NearZero(mMoveComp->GetForwardSpeed())
		&& mLastFootstep <= 0.f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}

	// ī�޶� ��ġ
	Vector3d cameraPos = GetPosition();
	// ī�޶� �� �� ��ġ
	Vector3d target = GetPosition() + GetForward() * 100.f;
	// ī�޶� �� ����(���������� �ƴ��� Ȯ���� �� �ְ�)
	Vector3d up = Vector3d::UnitZ;

	// �� ���
	Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);

	// ������� ���� ������ ����
	GetGame()->GetAudioSystem()->SetListener(view);
}

void CameraActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;

	// wasd�� �����̱�
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 300.0f;
	}
	// ȸ��
	if (keys[SDL_SCANCODE_A])
	{
		angularSpeed -= MathUtils::TwoPI;
	}
	if (keys[SDL_SCANCODE_D])
	{
		angularSpeed += MathUtils::TwoPI;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);

}

void CameraActor::SetFootstepSurface(float value)
{
	// ���⼭ �����.
	// �Ķ���� ���� ����Ǹ� �߼Ҹ��� ������ش�.
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}
