#include "PreCompiled.hpp"

PhysWorld::PhysWorld(Game* game)
	:mGame(game) {}

// 선분과 AABB 교차 함수를 활용하고
// 선분과 교차하는 평면의 법선 벡터를 반환하도록 수정한다.
bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl)
{
	bool collided = false;
	// closestT를 무한대 값으로 초기화한다.
	// 그래서 최초의 교차는 항상 closestT를 갱신한다.
	float closestT = MathUtils::Infinity;
	Vector3d norm;
	// 가장 가까운 교차를 얻기 위해
	// 모든 박스를 대상으로 테스트한다.
	for (auto box : mBoxes)
	{
		float t;
		// 선분과 박스가 교차하는지 테스트
		if (Intersect(l, box->GetWorldBox(), t, norm))
		{
			// 가장 작은 t값은 교차점이 선분의 시작점과 가장 가깝다는 것을 의미
			// 이 교차가 이전 교차보다 시작점에서 더 가까운지 확인
			if (t < closestT)
			{
				outColl.mPoint = l.PointOnSegment(t);
				outColl.mNormal = norm;
				outColl.mBox = box;
				outColl.mActor = box->GetOwner();
				collided = true;
			}
		}
	}

	return collided;
}

void PhysWorld::TestPairwise(std::function<void(Actor*, Actor*)> f)
{
	// 효율적이지 못한 구현 O(n^2)
	for (size_t i = 0; i < mBoxes.size(); i++)
	{
		// 이전 i 값 박스 컴포넌트와의 교차 테스트는 중복이므로 테스트할 필요 없다
		// 자신과의 교차 테스트도 불필요
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* a = mBoxes[i];
			BoxComponent* b = mBoxes[j];
			if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				// 교차 시 이를 처리하기 위한 함수를 호출
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}

}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> f)
{
	// min.x로 정렬
	std::sort(mBoxes.begin(), mBoxes.end(),
		[](BoxComponent* a, BoxComponent* b)
		{
			return a->GetWorldBox().mMin.x < b->GetWorldBox().mMin.x;
		});

	for (size_t i = 0; i < mBoxes.size(); i++)
	{
		// for box[i]의 max.x 값을 얻는다.
		BoxComponent* a = mBoxes[i];
		float max = a->GetWorldBox().mMax.x;
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* b = mBoxes[j];
			// box[j] min.x 값이 box[i]의 max.x 경곘값을 초과하면
			// box[i]와 가능한 충돌은 내부 루프에 존재하지 않는다.
			if (b->GetWorldBox().mMin.x > max)
			{
				break;
			}
			else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				f(a->GetOwner(), b->GetOwner());
			}
		}

	}
}

void PhysWorld::AddBox(BoxComponent* box)
{
	mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
	auto iter = std::find(mBoxes.begin(), mBoxes.end(), box);
	if (iter != mBoxes.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mBoxes.end() - 1);
		mBoxes.pop_back();
	}
}