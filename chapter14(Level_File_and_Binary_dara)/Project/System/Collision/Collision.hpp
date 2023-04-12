// ����
struct LineSegment
{
	LineSegment(const Vector3d& start, const Vector3d& end);
	// t���� �־����� �� ���л��� �� ���� ��ȯ�ϴ� �Լ�(0 <= t <= 1)
	// ����(0 <= t <= infinity), ����(-infinity <= t <= infinity)
	Vector3d PointOnSegment(float t) const;
	// ���� ���� �ִ� �Ÿ�^2
	float MinDistSq(const Vector3d& point) const;
	// �� ���а��� �ִ� �Ÿ�^2
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	// ���� ����
	Vector3d mStart;
	// �� ����
	Vector3d mEnd;
};


// ���
struct Plane
{
	Plane(const Vector3d& normal, float d);
	// �� ������ ��� ����
	Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c);
	// ��ȣ�� �ִ� ���� ����� �Ÿ� ���
	float SignedDist(const Vector3d& point) const;

	// ���� ����
	Vector3d mNormal;
	// ���� ���� ������ ��ȣ �ִ� �ּ� �Ÿ���
	float mD;
};

// ��ü �ٿ�� ����
struct Sphere
{
	Sphere(const Vector3d& center, float radius);
	// �浹 ����
	bool Contains(const Vector3d& point) const;

	// ��ü�� �߽�
	Vector3d mCenter;
	// ������
	float mRadius;

};

// �� ���� �ٿ�� �ڽ�(AABB, Axis-Aligned Bounding Box)
struct AABB
{
	AABB(const Vector3d& min, const Vector3d& max);
	// ���� ���� ���� min, max ����
	// ���� ���� �ε��ϴ� ��쿡 vertex�� �� ���
	void UpdateMinMax(const Vector3d& point);
	// ������Ʈ ȸ�� �� �ڽ� ����
	void Rotate(const Quaternion& q);
	// �浹 ����
	bool Contains(const Vector3d& point) const;
	// �Ķ���ͷ� ���� ���� AABB�� �ִ� �Ÿ�^2
	float MinDistSq(const Vector3d& point) const;

	// �ڽ��󿡼��� x,y,z �ּҰ�
	Vector3d mMin;
	// �ڽ��󿡼��� x,y,z �ִ밪
	Vector3d mMax;
};

// ���⼺ �ִ� �ٿ�� �ڽ�(OBB, Oriented Bounding Boxes)
struct OBB
{
	Vector3d mCenter;
	Quaternion mRotation;
	Vector3d mExtents;
};

// ĸ��
struct Capsule
{
	// ������ �ִ� ���а� ����.
	Capsule(const Vector3d& start, const Vector3d& end, float raidus);
	// t���� �־����� �� ���л��� �� ���� ��ȯ�ϴ� �Լ�(0 <= t <= 1)
	// ����(0 <= t <= infinity), ����(-infinity <= t <= infinity)
	Vector3d PointOnSegment(float t) const;
	bool Contains(const Vector3d& point) const;

	// ���, ����
	LineSegment mSegment;
	// ��ü�� ������, ĸ���� ������
	float mRaidus;
};

struct ConvexPolygon
{
	bool Contains(const Vector2d& point) const;
	// Vertices �� �ð� �������� ���ĵ��ִ�.
	std::vector<Vector2d> mVertices;
};

// ���� �׽�Ʈ
bool Intersect(const Sphere& a, const Sphere& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Capsule& a, const Capsule& b);
bool Intersect(const Sphere& s, const AABB& box);

bool Intersect(const LineSegment& l, const Sphere& s, float& outT);
bool Intersect(const LineSegment& l, const Plane& p, float& outT);

bool TestSidePlane(float start, float end, float negd, const Vector3d& norm,
	std::vector<std::pair<float, Vector3d>>& out);
bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3d& outNorm);

bool SweptSphere(const Sphere& P0, const Sphere& P1,
	const Sphere& Q0, const Sphere& Q1, float& outT);
