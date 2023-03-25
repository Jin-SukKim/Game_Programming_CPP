#include "PreCompiled.hpp"

CameraActor::CameraActor(Game* game)
	:Actor(game)
{
	mMoveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	// 카메라 위치
	Vector3d cameraPos = GetPosition();
	// 카메라 앞 점 위치
	Vector3d target = GetPosition() + GetForward() * 100.f;
	// 카메라 위 방향(뒤집혔는지 아닌지 확인할 수 있게)
	Vector3d up = Vector3d::UnitZ;

	// 뷰 행렬
	Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;

	// wasd로 움직이기
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 300.0f;
	}
	// 회전
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
