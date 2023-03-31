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
	// ���콺 ȸ��
	int x, y;
	// ��� ���콺
	SDL_GetRelativeMouseState(&x, &y);

	// ��Ŭ�� ���� ���� ȸ��
	if (state.Mouse.GetButtonState(SDL_BUTTON_RIGHT) == EHeld)
	{
		// ���콺 �������� -500~500�̶� ����
		const int maxMouseSpeed = 500;
		// �ִ� �ӵ��� �� rotation/sec
		const float maxOrbitSpeed = MathUtils::PI * 8;

		// �� ���
		float yawSpeed = 0.f;
		if (x != 0)
		{
			// [-1.0, 1.0] ����
			yawSpeed = static_cast<float>(x) / maxMouseSpeed;
			// rotation/sec ���ϱ�
			yawSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetYawSpeed(yawSpeed);

		// ��ġ ���
		float pitchSpeed = 0.f;
		if (y != 0)
		{
			// [-1.0, 1.0] ����
			pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
			// rotation/sec ���ϱ�
			pitchSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetPitchSpeed(pitchSpeed);
	}
}

void OrbitActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}