class PhysWorld
{
public:
	PhysWorld(class Game* game);

	// �浹 ����
	struct CollisionInfo
	{
		// �浹 ����
		Vector3d mPoint;
		// �浹 ���� ���� ����
		Vector3d mNormal;
		// �浹�� ������Ʈ
		class BoxComponent* mBox;
		// ������Ʈ�� ������ ����
		class Actor* mActor;
	};

	// ���а� �ڽ��� �浹 �׽�Ʈ
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	// ��� ���ڸ� ������� �׽�Ʈ�� ���� O(n^2) �˰���
	void TestPairwise(std::function<void(class Actor*, class Actor*)> f);
	// SAP �˰���, ������ �����׽�Ʈ ����ȭ O(n log n)
	void TestSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);

	// �ڽ� ������Ʈ �߰�/����
	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);

private:
	class Game* mGame;
	std::vector<class BoxComponent*> mBoxes;

};