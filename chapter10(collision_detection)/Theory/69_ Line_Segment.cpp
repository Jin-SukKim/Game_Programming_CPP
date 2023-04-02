/*
    기하 타입(geometric type)

    게임을 위한 충돌 감지에서는 기하나 선형대수 같은 여러 다양한 개념을 활용한다.
    일반적인 기본 기하 타입은 선분, 평면, 박스 등이다.

    선분(Line Segment)

    선분은 시작 지점과 끝 지점으로 구성된다.
    
    struct LineSegment
    {
        Vector3d mStart;
        Vector3d mEnd;
    };

    선분상의 임의의 점을 계산하려면 다음 매개 방정식을 사용하면 되는데,
    Start와 End는 각각 시작 지점과 끝 지점이고 t는 파라미터다.

    L(t) = Start + (End - Start) * t, where 0 <= t <= 1

    편의성을 위해 t값이 주어졌을 때 선분상의 한 점을 반환하는 멤버 함수를 LineSegment 클래스에 추가한다.

    선분의 매개 표현식은 손쉽게 광선이나 선의 정의로 확장할 수 있다.
    광선은 위의 방정식을 따르지만 t값의 경계는 다음과 같다.

    0 <= t <= infinity

    선은 t값에 경계가 없다

    -infinity <= t <= infinity

    선분과 광선은 여러 다양한 유형의 충돌 감지를 위한 쓰임새가 많은 기본 기하타입이다.
    ex) 총알을 발사하거나 땅에 착지하는 경우는 선분을 사용
        조준용 십자선은 위해 선분을 사용하며, 소리 차폐 테스트, 마우스 피킹에도 사용한다.

    또 하나의 유용한 연산은 임의이 한 점과 선분 사이의 최소 거리를 찾는 것이다.
    선분이 점 A에서 시작하고 점 B에서 끝난다고 가정할 때, 임의의 점 C가 주어졌을 때 선분과
    점 C 사이의 최소 거리를 구하려면 3가지의 다른 경우를 고려해야 한다.

    1. AB와 AC 사이의 각이 90도보다 큰 경우.
        내적을 사용하면 90도보다 큰지를 확인하는 것이 가능하다.
        내적이 음수이면 두 벡터 사이의 각이 둔각임을 의미하므로 C와
        선분 사이의 최소 거리는 벡터 AC의 길이가 된다.

    2. BA와 BC 사이의 각이 90도보다 큰 경우.
        내적으로 각도 확인 가능하다.
        최소 거리는 BC의 길이가 된다.
    
    3. AB에서 AB에 수직하는 C로의 선분을 그린다.
        이 새로운 수직 선분의 길이는 C와 AB 사이의 최소 거리다.
        이 선분을 구하려면 먼저 벡터 p를 계산해야 한다.
        
        p의 방향은 정규화된 AB와 같은 방향이므로 이미 알고 있다.
        스칼라 투영(scalar projection)이란 내적의 특성을 사용해 p를 구한다.
        스칼라 투영은 직교 투영을 의미하고 선분 AC를 선분 AB로 직교 투영하는 것을 뜻한다.
        비단위 벡터 AB는 직각삼각형을 구성하기 위해 확장되거나 축소되야 한다.

        즉 선분 AC가 선분 AB로 직교 투영된, 벡터 p의 길이는 AC와 정규화된 AB(AB의 단위 벡터)의 내적이된다.

        ||p|| = AC * (AB / ||AB||), * = 내적

        cos각도 = ||p|| / ||AC||
        AC * (AB / ||AB||) = ||AC|| * cos각도 = ||p||

        여기서 각도는 AB와 AC사이의 각이다.

        결국 벡터 p는 정규화된 AB 벡터와 p의 길이의 스칼라 곱이 된다.

        p = ||p|| * (AB / ||AB||)

        벡터의 제곱 길이와 내적은 같다는 점을 이용해 몇 가지 대수적 계산을 사용하면 p는
        다음과 같이 간소화된다.

        p = (AC * AB, 내적) / (AB * AB, 내적) * AB, 그냥 곱하기

        마지막으로 p에서 AC로의 벡터를 선언한다.
        이 벡터의 길이는 AB와 C까지의 최소 거리다.

        d = ||AC - p||

        거리는 양수값이며 AB에서 C까지의 최소 거리의 제곱값을 얻기 위해 방정식 양쪽을 제곱한다.
        그래서 비용이 큰 제곱근 연산을 피한다.

        d^2 = ||AC - p||^2
*/

/*
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
*/