#include "PreCompiled.hpp"

CameraActor::CameraActor(Game* game)
	:Actor(game)
{
	mMoveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	// ī�޶� ��ġ
	Vector3d cameraPos = GetPosition();
	// ī�޶� �� �� ��ġ
	Vector3d target = GetPosition() + GetForward() * 100.f;
	// ī�޶� �� ����(���������� �ƴ��� Ȯ���� �� �ְ�)
	Vector3d up = Vector3d::UnitZ;

	// �� ���
	Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
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
