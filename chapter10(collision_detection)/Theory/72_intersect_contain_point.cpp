/*
    교차 테스트

    오브젝트의 충돌을 표현하기 위해 여러 기하타입을 사용하는 방법을 배웠다.
    그 후는 이 오브젝트 간 교차 여부를 테스트해야 한다.

    여러 유용한 교차 테스트가 있다.
    
    1. 오브젝트가 점을 포함하는지를 판정하는 방법
    2. 여러 타입의 바운딩 볼륨 간 교차
    3. 동적으로 움직이는 오브젝트를 다루는 방법

    점을 포함하는지 판정하는 테스트

    물체가 특정한 점을 포함하는지 여부를 테스트하는 것이다.
    다양하게 사용되는데 플레이어가 세계의 특정 지역 내부에 있는지 여부를 판단하는 테스트
    등에 사용된다.
    그리고 일부 물체의 교차 알고리즘은 물체에 근접한 점을찾고,
    그 점이 물체 내부에 있는지를 알아내는 데 의존한다.
    이번 공부는 점이 해당 물체의 가장자리에 있어도 물체에 포함된 것으로 판단한다.

    구의 점 포함 판정 테스트.

    먼저점과 구의 중심 사이의 거리를 구한다.
    이 거리가 구의 반지름보다 같거나 작다면 구는 점을 포함한다.

    거리와 반지름은 둘 다양수이므로 젝보한값을 사용해도
    비교 연산을 항등성을 유지하므로 비용이 큰 제곱근 연산을 피한다.

    bool Sphere::Contains(const Vector3d& point) const
    {
        // 구체의 중심에서 점까지의 거리
        float distSq = (mCenter - point).LengthSq();
        return distSq <= (mRadius * mRadius);
    }

    AABB 점 포함 판정 테스트

    2D AABB에 대해선 점은 다음과 같은 경우가 참이면 박스 바깥에 있다.

    1. 점이 박스 왼쪽에 있다.
    2. 점이 박스 오른쪽에 있다.
    3. 점이 박스 위쪽에 있다.
    4. 점이 박스 아래쪽에 있다.

    이 경우에 해당하지 않으면 박스는 점을 포함해있다.

    이 테스트는 박스의 min, max 점과 박스 내부에 존재하는지를 확인할 점의
    요소를 비교하면된다.

    이 개념은 3D AABB에도 쉽게 확장할 수 있다.
    2D 박스는네번의 측면 검사가 필요한 반면 3D AABB는 6개의 면이 존재하므로 6번의 검사가 필요하다.

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

    캡슐의 점 포함 테스트

    캡슐이 점을 포함하는지 유무를 테스트하려면 먼저 점과 선분 사이의 최서 거리 제곱값을 계산해야 한다.
    최소 거리제곱값 계산을 위해 LineSegment::MinDistSq 함수를 사용하자.
    최소 거리제곱값이 반지름 제곱값 보다 같거나 작으면 캡슐은 점을 포함한다.

    bool Capsule::Contains(const Vector3d& point) const
    {
        // 점과 선분의 최단거리
        float distSq = mSegment.MinDistSq(point);
        return distSq <= (mRaidus * mRaidus);
    }
    
    볼록 다각형의 점 포함(2D) 테스트

    2D 다각형이 점을 포함하는지를 테스트하는 방법은 여러가지가 있따.
    가장 간단한 접근법 중 한 가지는 점과 각 vertex 간 vector를 만드는 것이다.
    그리고 인접한 두 벡터 간 내적을 한 뒤 이 두 벡터가 이루는 각을 구하기 위해 arcosine 계산을 한다.
    이 각들의 모든 합이 360도에 가까우면 점은 폴리곤 내부에 있는 것이다.
    그렇지 않다면 점은 바깥에 있따.

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

        // 2PI에 가깝다면 true를 반환(360도는 2PI와 같다.)
        return MathUtils::NearZero(sum - MathUtils::TwoPI);
    }

    제곱근과 acos 계산을 요구하기에 이 각의 총합 접근법은 효율적이지 못하다.
    보다 복잡하지만 다른 접근법 중에는 각의 총합 접근법보다 효율적인데 많다.
    그 중 한가지는 점에서 시작하는 무한한 광선을 그리고, 광선이 모서리와
    몇 개나 교차하는 지를 세는 것이다.
    광선이 모서리와 홀수로 교차한다면 점은 폴리곤 내부에 있고 아니면 바깥에 있다.
    이 광선 접근법은 볼록 다각형뿐만 아닌 오목 다각형에서도 잘 동작한다

*/