// 선분
struct LineSegment
{
	LineSegment(const Vector3d& start, const Vector3d& end);
	// t값이 주어졌을 때 선분상의 한 점을 반환하는 함수(0 <= t <= 1)
	// 광선(0 <= t <= infinity), 선분(-infinity <= t <= infinity)
	Vector3d PointOnSegment(float t) const;
	// 점과 선의 최단 거리^2
	float MinDistSq(const Vector3d& point) const;
	// 두 선분간의 최단 거리^2
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	// 시작 지점
	Vector3d mStart;
	// 끝 지점
	Vector3d mEnd;
};


// 평면
struct Plane
{
	Plane(const Vector3d& normal, float d);
	// 세 점으로 평면 생성
	Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c);
	// 부호가 있는 점과 평면의 거리 계산
	float SignedDist(const Vector3d& point) const;

	// 법선 벡터
	Vector3d mNormal;
	// 평면과 원점 사이의 부호 있는 최소 거리값
	float mD;
};

// 구체 바운딩 볼륨
struct Sphere
{
	Sphere(const Vector3d& center, float radius);
	// 충돌 감지
	bool Contains(const Vector3d& point) const;

	// 구체의 중심
	Vector3d mCenter;
	// 반지름
	float mRadius;

};

// 축 정렬 바운딩 박스(AABB, Axis-Aligned Bounding Box)
struct AABB
{
	AABB(const Vector3d& min, const Vector3d& max);
	// 점의 값에 따라 min, max 갱신
	// 보통 모델을 로딩하는 경우에 vertex를 얻어서 사용
	void UpdateMinMax(const Vector3d& point);
	// 오브젝트 회전 후 박스 갱신
	void Rotate(const Quaternion& q);
	// 충돌 감지
	bool Contains(const Vector3d& point) const;
	// 파라미터로 받은 점과 AABB의 최단 거리^2
	float MinDistSq(const Vector3d& point) const;

	// 박스상에서의 x,y,z 최소값
	Vector3d mMin;
	// 박스상에서의 x,y,z 최대값
	Vector3d mMax;
};

// 방향성 있는 바운딩 박스(OBB, Oriented Bounding Boxes)
struct OBB
{
	Vector3d mCenter;
	Quaternion mRotation;
	Vector3d mExtents;
};

// 캡슐
struct Capsule
{
	// 반지름 있는 선분과 같다.
	Capsule(const Vector3d& start, const Vector3d& end, float raidus);
	// t값이 주어졌을 때 선분상의 한 점을 반환하는 함수(0 <= t <= 1)
	// 광선(0 <= t <= infinity), 선분(-infinity <= t <= infinity)
	Vector3d PointOnSegment(float t) const;
	bool Contains(const Vector3d& point) const;

	// 경로, 선분
	LineSegment mSegment;
	// 구체의 반지름, 캡슐의 반지름
	float mRaidus;
};

struct ConvexPolygon
{
	bool Contains(const Vector2d& point) const;
	// Vertices 는 시계 방향으로 정렬돼있다.
	std::vector<Vector2d> mVertices;
};

// 교차 테스트
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
