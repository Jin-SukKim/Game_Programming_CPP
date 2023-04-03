#include "PreCompiled.hpp"

BallMove::BallMove(Actor* owner)
	:MoveComponent(owner) {}

void BallMove::Update(float deltaTime)
{
	// �̵� �������� ������ �����Ѵ�.
	const float segmentLength = 30.0f;
	Vector3d start = mOwner->GetPosition();
	Vector3d dir = mOwner->GetForward();
	Vector3d end = start + dir * segmentLength;

	// ���� ����
	LineSegment ls(start, end);

	// ���а� ���� ������� �׽�Ʈ ����
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	// �÷��̾�� �浹���� �ʰ� ����
	if (phys->SegmentCast(ls, info) && info.mActor != mPlayer)
	{
		// �浹�ߴٸ� ǥ�� ������ �������� �ؼ� �ݻ�
		dir = Vector3d::Reflect(dir, info.mNormal);
		SDL_Log("%f %f %f", info.mNormal.x, info.mNormal.y, info.mNormal.z);
		mOwner->RotateToNewForward(dir);

		// Ÿ���� ����ٸ�
		TargetActor* target = dynamic_cast<TargetActor*>(info.mActor);
		if (target)
		{
			static_cast<BallActor*>(mOwner)->HitTarget();
		}
	}

	// ���� �ӵ��� ��Ÿ �ð����� ���̽� Ŭ���� �̵��� ����
	MoveComponent::Update(deltaTime);
}