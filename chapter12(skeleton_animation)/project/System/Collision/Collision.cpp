#include "..\..\PreCompiled.hpp"

LineSegment::LineSegment(const Vector3d& start, const Vector3d& end)
	:mStart(start), mEnd(end) {}

Vector3d LineSegment::PointOnSegment(float t) const
{
	return mStart + (mEnd - mStart) * t;
}

float LineSegment::MinDistSq(const Vector3d& point) const
{
	// 벡터 선언
	Vector3d ab = mEnd - mStart;
	Vector3d ba = ab * -1.f;
	Vector3d ac = point - mStart;
	Vector3d bc = point - mEnd;

	// 케이스 1 : C와 A의 거리를 구하는 경우
	if (Vector3d::Dot(ab, ac) < 0.f)
	{
		return ac.LengthSq();
	}

	// 케이스 2 : C와 B의 거리를 구하는 경우
	else if (Vector3d::Dot(ba, bc) < 0.f)
	{
		return bc.LengthSq();
	}

	// 케이스 3: C를 선분에 투영해서 최소 거리를 구하는 경우
	else
	{
		// p를 계산, 투영 벡터
		float scalar = Vector3d::Dot(ac, ab) / Vector3d::Dot(ab, ab);
		Vector3d p = ab * scalar;
		// ac - p 길이 제곱값 계산
		return (ac - p).LengthSq();
	}
}

float LineSegment::MinDistSq(const LineSegment& s1, const LineSegment& s2)
{
	Vector3d u = s1.mEnd - s1.mStart;
	Vector3d v = s2.mEnd - s2.mStart;
	Vector3d w = s1.mStart - s2.mStart;	// 두 선분의 시작점 사이 벡터
	// 벡터가 평행한 경우 내적은 벡터의 제곱과 같다.
	float    a = Vector3d::Dot(u, u);         // always >= 0
	float    b = Vector3d::Dot(u, v);
	float    c = Vector3d::Dot(v, v);         // always >= 0
	float    d = Vector3d::Dot(u, w);
	float    e = Vector3d::Dot(v, w);

	float    D = a * c - b * b;        // always >= 0
	float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
	float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

	// compute the line parameters of the two closest points
	if (MathUtils::NearZero(D)) { // the lines are almost parallel
		sN = 0.0;         // force using point P0 on segment S1
		sD = 1.0;         // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else {                 // get the closest points on the infinite lines
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (MathUtils::NearZero(sN) ? 0.0f : sN / sD);
	tc = (MathUtils::NearZero(tN) ? 0.0f : tN / tD);

	// get the difference of the two closest points
	Vector3d dP = w + (u * sc) - (v * tc);  // =  S1(sc) - S2(tc)

	return dP.LengthSq();   // return the closest distance squared
}

Plane::Plane(const Vector3d& normal, float d)
	:mNormal(normal), mD(d) {}

Plane::Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c)
{
	// a에서 b, a에서 c로의 벡터를 계산
	Vector3d ab = b - a;
	Vector3d ac = c - a;
	// 법선 벡터를 얻기 위해 외적을 한 뒤 정규화
	mNormal = Vector3d::Cross(ab, ac);
	mNormal.Normalize();
	// d = -P dot n
	mD = -Vector3d::Dot(a, mNormal);
}

float Plane::SignedDist(const Vector3d& point) const
{
	return Vector3d::Dot(point, mNormal) - mD;
}

Sphere::Sphere(const Vector3d& center, float radius) :mCenter(center), mRadius(radius) {}

bool Sphere::Contains(const Vector3d& point) const
{
	// 구체의 중심에서 점까지의 거리
	float distSq = (mCenter - point).LengthSq();
	return distSq <= (mRadius * mRadius);
}

AABB::AABB(const Vector3d& min, const Vector3d& max) : mMin(min), mMax(max) {}

void AABB::UpdateMinMax(const Vector3d& point)
{
	// 각 요솟값을 별도로 갱신
	mMin.x = MathUtils::Min(mMin.x, point.x);
	mMin.y = MathUtils::Min(mMin.y, point.y);
	mMin.z = MathUtils::Min(mMin.z, point.z);

	mMax.x = MathUtils::Max(mMax.x, point.x);
	mMax.y = MathUtils::Max(mMax.y, point.y);
	mMax.z = MathUtils::Max(mMax.z, point.z);
}

void AABB::Rotate(const Quaternion& q)
{
	// 박스의 구석 8개 점을 생성
	std::array<Vector3d, 8> points;
	// Min 점은 항상 구석
	points[0] = mMin;
	// Min, Max 점을 제외한 나머지 구석은 Min점과 Max점 요소 조합으로 구한다.
	// 2개의 min, 1 max 순열로 구석점 생성
	points[1] = Vector3d(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3d(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3d(mMin.x, mMin.y, mMax.z);
	// 2개의 max, 1개의 min 순열로 구석점 생성
	points[4] = Vector3d(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3d(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3d(mMax.x, mMax.y, mMin.z);
	// max 구석점
	points[7] = Vector3d(mMax);

	// 최초 점을 쿼터니언으로 회전시킴
	Vector3d p = Vector3d::Transform(points[0], q);
	// 회전된 최초의 점으로 min/max 리셋
	mMin = p;
	mMax = p;
	// 남아 있는 점들을 회전시킨 뒤 min/max 갱신
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3d::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

bool AABB::Contains(const Vector3d& point) const
{
	bool outside =
		point.x < mMin.x ||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;
	return !outside;
}

float AABB::MinDistSq(const Vector3d& point) const
{
	// 각각의 축과 점 사이의 거리를 계산한다.
	float dx = MathUtils::Max(mMin.x - point.x, 0.0f);
	dx = MathUtils::Max(dx, point.x - mMax.x);
	float dy = MathUtils::Max(mMin.y - point.y, 0.0f);
	dy = MathUtils::Max(dy, point.y - mMax.y);
	float dz = MathUtils::Max(mMin.z - point.z, 0.0f);
	dz = MathUtils::Max(dy, point.z - mMax.z);

	// 거리의 제곱 공식
	return dx * dx + dy * dy + dz * dz;
}

Capsule::Capsule(const Vector3d& start, const Vector3d& end, float radius)
	:mSegment(start, end), mRaidus(radius) {}


Vector3d Capsule::PointOnSegment(float t) const
{
	return mSegment.PointOnSegment(t);
}

bool Capsule::Contains(const Vector3d& point) const
{
	// 점과 선분의 최단거리
	float distSq = mSegment.MinDistSq(point);
	return distSq <= (mRaidus * mRaidus);
}

bool ConvexPolygon::Contains(const Vector2d& point) const
{
	float sum = 0.f;
	Vector2d a, b;
	for (size_t i = 0; i < mVertices.size() - 1; i++)
	{
		// 점에서 첫번째 vertex까지의 벡터
		a = mVertices[i] - point;
		a.Normalize();

		// 점에서 두번째 vertex까지의 벡터
		b = mVertices[i + 1] - point;
		b.Normalize();
		// 두 벡터로 구한 각도 더하기
		sum += MathUtils::Acos(Vector2d::Dot(a, b));
	}

	// 마지막 vertex와 첫 vertex의 점까지의 각도 더하기
	a = mVertices.back() - point;
	a.Normalize();
	b = mVertices.front() - point;
	b.Normalize();
	sum += MathUtils::Acos(Vector2d::Dot(a, b));

	// 2PI에 가깝다면 true를 반환
	return MathUtils::NearZero(sum - MathUtils::TwoPI);
}

bool Intersect(const Sphere& a, const Sphere& b)
{
	float distSq = (a.mCenter - b.mCenter).LengthSq();
	float sumRad = a.mRadius + b.mRadius;
	return distSq <= (sumRad * sumRad);
}

bool Intersect(const AABB& a, const AABB& b)
{
	bool no =
		a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;
	return !no;
}

bool Intersect(const Sphere& s, const AABB& box)
{
	float distSq = box.MinDistSq(s.mCenter);
	return distSq <= (s.mRadius * s.mRadius);
}

bool Intersect(const Capsule& a, const Capsule& b)
{
	float distSq = LineSegment::MinDistSq(a.mSegment, b.mSegment);
	float sumRad = a.mRaidus + b.mRaidus;
	return distSq <= (sumRad * sumRad);
}

bool Intersect(const LineSegment& l, const Plane& p, float& outT)
{
	// 분모값을 확인해 t의 해가 존재하는지 확인(내적이 0인지 확인)
	float denom = Vector3d::Dot(l.mEnd - l.mStart, p.mNormal);

	if (MathUtils::NearZero(denom)) // 분모가 0에 가깝다면
	{
		// 선과 평면이 교차하는 유일한 길은 두 점이 모두 평면상의 점인 경우다.
		// 즉 (P dot N) == d 평면의 방정식을 만족해야 한다.
		if (MathUtils::NearZero(Vector3d::Dot(l.mStart, p.mNormal) - p.mD))
		{
			outT = 0.f;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		float numer = -Vector3d::Dot(l.mStart, p.mNormal) - p.mD;
		outT = numer / denom;
		// t가 선분 범위 내의 값인지 검증
		if (outT >= 0.f && outT <= 1.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Intersect(const LineSegment& l, const Sphere& s, float& outT)
{
	// 방정식에 등장한 X, Y, a, b, c를 계산
	Vector3d X = l.mStart - s.mCenter;
	Vector3d Y = l.mEnd - l.mStart;
	float a = Vector3d::Dot(Y, Y);
	float b = 2.f * Vector3d::Dot(X, Y);
	float c = Vector3d::Dot(X, X) - s.mRadius * s.mRadius;
	// 판별식을 계산
	float disc = b * b - 4.f * a * c;
	if (disc < 0.f) // 교차하지 않음
	{
		return false;
	}
	else
	{
		disc = MathUtils::Sqrt(disc);
		// t의 min / max 해를 계산
		float tMin = (-b - disc) / (2.f * a);
		float tMax = (-b + disc) / (2.f * a);
		// t의 값이 선분 범위 내에 있는지 검사
		if (tMin >= 0.f && tMin <= 1.f)
		{
			outT = tMin;
			return true;
		}
		else if (tMax >= 0.f && tMax <= 1.f)
		{
			outT = tMax;
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool TestSidePlane(float start, float end, float negd, const Vector3d& norm,
	std::vector<std::pair<float, Vector3d>>& out)
{
	float denom = end - start;
	if (MathUtils::NearZero(denom))
	{
		return false;
	}
	else
	{
		float numer = -start + negd;
		float t = numer / denom;
		// t값이 범위 내에 있는지 검사
		if (t >= 0.f && t <= 1.f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}
bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3d& outNorm)
{
	// 교차 가능성 있는 모든 t값과 법선 벡터를 저장하는 벡터 선언
	std::vector<std::pair<float, Vector3d>> tValues;

	// x축에 수직한 평면 2개와 선분간 교차 확인
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, Vector3d::NegUnitX, tValues);
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, Vector3d::UnitX, tValues);
	// y축에 수직한 평면 2개와 선분간 교차 확인
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, Vector3d::NegUnitY, tValues);
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, Vector3d::UnitY, tValues);
	// z축에 수직한 평면 2개와 선분간 교차 확인
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, Vector3d::NegUnitZ, tValues);
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, Vector3d::UnitZ, tValues);

	// 오름차순으로 t값 정렬
	std::sort(tValues.begin(), tValues.end(),
		[](const std::pair<float, Vector3d>& a,
			const std::pair<float, Vector3d>& b)
		{
			return a.first < b.first;
		});
	// 박스가 이 교차점들을 포함하는지 확인
	Vector3d point;
	for (auto& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (b.Contains(point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}

	// 박스과 교차하는 점이 하나도 없다.
	return false;
}


bool SweptSphere(const Sphere& P0, const Sphere& P1,
	const Sphere& Q0, const Sphere& Q1, float& outT)
{
	// X, Y, a, b, c를 계산
	Vector3d X = P0.mCenter - Q0.mCenter;
	Vector3d Y = P1.mCenter - P0.mCenter - (Q1.mCenter - Q0.mCenter);
	float a = Vector3d::Dot(Y, Y);
	float b = 2.f * Vector3d::Dot(X, Y);
	float sumRad = P0.mRadius + Q0.mRadius;
	float c = Vector3d::Dot(X, X) - sumRad * sumRad;
	// 판별식을 계산한다.
	float disc = b * b - 4.f * a * c;
	if (disc < 0.f)
	{
		return false;
	}
	else
	{
		disc = MathUtils::Sqrt(disc);
		// 최초 충돌만이 의미가 있으므로 2개의 해 중 작은 해만 다룬다
		outT = (-b - disc) / (2.f * a);
		if (outT >= 0.f && outT <= 0.f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}