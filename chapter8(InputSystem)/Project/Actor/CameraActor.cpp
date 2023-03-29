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

void CameraActor::ActorInput(const InputState& state)
{
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;

	// wasd�� �����̱�
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_W) == EHeld)
	{
		forwardSpeed += 300.0f;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_S) == EHeld)
	{
		forwardSpeed -= 300.0f;
	}
	// ȸ��
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
	// ���⼭ �����.
	// �Ķ���� ���� ����Ǹ� �߼Ҹ��� ������ش�.
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}
