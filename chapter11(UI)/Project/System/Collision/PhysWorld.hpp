class PhysWorld
{
public:
	PhysWorld(class Game* game);

	// 충돌 정보
	struct CollisionInfo
	{
		// 충돌 지점
		Vector3d mPoint;
		// 충돌 시의 법선 벡터
		Vector3d mNormal;
		// 충돌한 컴포넌트
		class BoxComponent* mBox;
		// 컴포넌트의 소유자 액터
		class Actor* mActor;
	};

	// 선분과 박스의 충돌 테스트
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	// 모든 상자를 대상으로 테스트를 수행 O(n^2) 알고리즘
	void TestPairwise(std::function<void(class Actor*, class Actor*)> f);
	// SAP 알고리즘, 상자의 교차테스트 최적화 O(n log n)
	void TestSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);

	// 박스 컴포넌트 추가/제거
	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);

private:
	class Game* mGame;
	std::vector<class BoxComponent*> mBoxes;

};