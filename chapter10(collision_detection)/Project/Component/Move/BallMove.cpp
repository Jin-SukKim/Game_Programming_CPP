#include "PreCompiled.hpp"

BallMove::BallMove(Actor* owner)
	:MoveComponent(owner) {}

void BallMove::Update(float deltaTime)
{
	// 이동 방향으로 선분을 생성한다.
	const float segmentLength = 30.0f;
	Vector3d start = mOwner->GetPosition();
	Vector3d dir = mOwner->GetForward();
	Vector3d end = start + dir * segmentLength;

	// 선분 생성
	LineSegment ls(start, end);

	// 선분과 게임 세계와의 테스트 진행
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	// 플레이어와 충돌하지 않게 설정
	if (phys->SegmentCast(ls, info) && info.mActor != mPlayer)
	{
		// 충돌했다면 표면 법선을 기준으로 해서 반사
		dir = Vector3d::Reflect(dir, info.mNormal);
		SDL_Log("%f %f %f", info.mNormal.x, info.mNormal.y, info.mNormal.z);
		mOwner->RotateToNewForward(dir);

		// 타겟을 맞췄다면
		TargetActor* target = dynamic_cast<TargetActor*>(info.mActor);
		if (target)
		{
			static_cast<BallActor*>(mOwner)->HitTarget();
		}
	}

	// 전진 속도와 델타 시간으로 베이스 클래스 이동을 갱신
	MoveComponent::Update(deltaTime);
}