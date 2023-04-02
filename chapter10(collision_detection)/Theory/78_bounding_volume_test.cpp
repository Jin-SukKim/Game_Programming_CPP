/*
    바운딩 볼륨 테스트

    여러 바운딩 볼륨 간 교차 테스트 계산은 매우 일반적이다.
    플레이어와벽의 충돌 검사를 위해 둘 다 AABB 사용시 두 AABB가 교차하는지를 테스트한다
    그리고 플레이어와 벽이 교차한다면 플레이어 위치를 더 이상교차하지 않도록 수정한다.

    지금은 모든 교차를 다루지는 않지만 중요한 교차 몇가지를 공부한다.

    구체와 구체 교차 테스트

    두 구체는 구체의 중심 사이의 거리가 각 구체의 반지름 합보다 작거나 같으면 교차한다.
    구체의 점 포함 테스트처험 효율성을 위해 양변을 제곱해도 항등성이 유지되는걸 이용한다.

    bool Intersect(const Sphere& a, const Sphere& b)
    {
        float distSq = (a.mCenter - b.mCenter).LengthSq();
        float sumRad = a.mRadius + b.mRadius;
        return distSq <= (sumRad * sumRad);
    }

    AABB와 AABB 교차 테스트

    AABB 교차를 시험하는 로직은 AABB가 점을 포함하는지를 판단하는 로직과 같다.
    두 AABB가 교차할 수 없는 경우를 테스트한다.
    이 테스트가 모두 참이 아니면 두 AABB는 교차해야 한다.
    2D AABB 교차 테스트, 박스 A, B

    두 박스가 교차하지 않고 서로 좌우에 있는 경우
    1. A.max.x < B.min.x
    2. B.max.x < A.min.x

    두 박스가 교차하지 않고 위아래 있는 경우
    3. A.max.y < B.min.y
    4. B.max.y < A.min.y

    3D AABB의 교차 테스트로 변경시엔 6개 요소를 다 검사한다.
    이런 형태의 AABB 교차는 두 볼록한 오브젝트가 교차하지 않으면 A와 B를 구분하는 축이
    존재해야 함을 나타내는 분리 축 이론(separating axis theorem)을 적용한 것이다.
    AABB의 경우 3개의 축으로 박스 사이에 분리가 있는지를 테스트하고 있다.
    AABB가 특정 축으로 분리가 된다면 분리 축 정지에 따라 두 박스느교차할 수 없다.
    이 접근법은 OBB로도 확장가능하며 모든 볼록한 오브젝트에 적용 가능하다.

    구와 AABB 교차 테스트

    이 테스트는 먼저 구의 중심과 박스 사이의 최소 거리 제곱값을 계산해야 한다.
    점과 AABB 사이의 최소 거리를 찾는 알고리즘은각 요소를 개별적으로 테스트한다.
    3가지 테스트 경우의 수가 존재한다.

    1. 점의 요소가 min보다 작다.
    2. 점의 요소가 min, max 사이에 있다.
    3. 점의 요소가 max보다 크다.

    두번째 경우 점과 박스 사이의 거리는 0이다.
    다른 2가지의 경우 점과 박스 사이의 거리는 가장 가까운 모서리와의 거리다.
    (또는 점과 min, max와의 거리.)

    Max 함수를 여러 번 호출해 거리를 구한다.
    ex) x 방향에서의거리

        float dx = MathUtils::Max(mMin.x - point.x, 0.f);
        dx = MathUtils::(dx, point.x - mMax.x);

        이 코드는 point.x < min.x 이면 min.x - point.x는 세 값 중에서 가장 크며,
        x축에 대한 델타 값이 된다.
        그렇지 않고 min.x < point.x < max.x 라면 0이 가장 크다.
        마지막으로 point.x > max.x 라면 point.x - max.x가 가장 크다.
    
    세 축 모두에 대한 델타를 어등면 점과 AABB사이의 최종 거리 제곱값을 계산하는 거리 공식을 사용한다.

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

    MinDistSq 함수 구현으로 구체와 AABB 간 교차 테스트가 가능해졌다.
    구체의 중심과 AABB의 최소 거리 제곱ㅇ르 알아내고 그 값이 반지름의 제곱보다
    작거나 같다면 구와 AABB는 교차한다.

    bool Intersect(const Sphere& s, const AABB& box)
    {
        float distSq = box.MinDistSq(s.mCenter);
        return distSq <= (s.mRadius * s.mRadius);
    }

    캡슐과 캡슐 교차 테스트

    두 캡슐의 교차는 개념적으로 간단하다.
    캡슐은 반경이 있는 선분이므로 먼저 이 선분 사이의 최소 거리 제곱값을 구한다.
    최소 거리 제곱값이 선분들의 반경의 합 제곱보다 작거나 같다면 두 캡슐은 교차한다.

    bool Intersect(const Capsule& a, const Capsule& b)
    {
        float distSq = LineSegment::MinDistSq(a.mSegment, b.mSegment);
        float sumRad = a.mRaidus + b.mRaidus;
        return distSq <= (sumRad * sumRad);
    }

    두 선분 사이의 최소 거리 계산은 몇몇 가장자리의 경우 때문에 복잡하다.
    지금은 깊이 파고들지 않는다.
*/