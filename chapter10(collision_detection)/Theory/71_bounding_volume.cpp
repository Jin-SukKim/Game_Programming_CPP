/*
    바운딩 볼륨(bounding volume)

    현대 3D 게임은 수천 개의 삼각형으로 구성된 캐릭터와 오브젝트로 표현된다.
    두 오브젝트의 충돌 여부를 결정하고자 할 때오브젝트를 구성하는 모든 삼각형의
    교차 여부를 테스트하는 것은 비효율적이며, 이런 이유료 게임에선 박스나 구체같은
    간소화된 바운딩 볼륨을 사용한다.
    두 오브젝트의 교차 판단을 위한 단순화된 충돌 계싼을 통해 게임은 매우 향상된 효율성을 얻는다

    구체(Sphere)

    3D 오브젝트의 가장 단순화된 경계 표현은 구체다.
    정의에 구체의중심과 반지름이 필요하다.

    struct Sphere
    {
        Vector3d mCenter;
        float mRaidus;
    }

    바운딩 구체는 일부 오브젝트에는 적합하지만, 다른 오브젝트에는 적합하지 않다.
    ex) 인간형 캐릭터는 캐릭터와 구체 경계 사이에 빈 공간이 많다.
    물체에 대한 경계 부분이 헐렁하므로 두 물체의 바운딩 볼륨이 교차해도
    실제로는 충돌하지 않았음을 의미하는 긍정 오류(false positive)가 발생한다.

    그러나 바운딩 구체를 사용하면 교차 계산이 매우 효율적이라는 이점을 가진다.
    게다가 회전은 구체에 영향을 주지 않아 3D 오브젝트의 회전에 상관없이 잘 동작한다.

    축 정렬 바운딩 박스(AABB, Axis-Aligned Bounding Box)

    2D에서 AABB는 모서리가 x축과 y축에 평행한 사각형이다.
    비슷하게 3D에서 AABB는 프리즘의 모든 면이 좌표축 평면의 하나와 평행한 사각형 프리즘이다.

    AABB는 최소점과 최대점 두 점으로 정의한다.
    2D에선 왼쪽 하단점이 최소점, 오른쪽 상단점이 최대점이다.
    즉, 최소점은 박스상에서 최솟값 x, y이고 최대점은 박스상에서 x, y 최대값이다.

    3D에서 최소점은 박스상에서 x, y, z의 최솟값을 가지며, 최대점은 x,y,z 최대값을 가진다.

    struct AABB
    {
        Vector3d mMin;
        Vector3d mMax;
    }

    AABB는 여러 점들을 이요하면 쉽게 생성할 수 있다.
    모델을 로딩하는 경우 일련의 vertex를 얻을 수 있고, 이 vertex 시퀸스를 모델에 대한
    AABB를 정의하는데 사용 가능하다.
    이를 위해 점ㅇ르 파라미터로 받는 UpdateMinMax 함수를 구현한다.

    UpdateMinMax 함수
    새로운 점이 모든 다른 점에 대해 상대적으로 어디에 위치하는지를 알 수 없으므로
    min, max값 갱신을 위해 모든 요소를 개별적으로 테스트해야 한다.
    (x, y, z를 각각 따로 비교해준다.)

    점 리스트를 가진 컨테이너가 주어지면 먼저 컨테이너의 첫 번째 점값으로 AABB의 min, max를 초기화한다.
    첫 번째 점을 제외한 나머지 점들에 대해서는 단순히 UpdateMinMax 함수를 호출한다.

    AABB는 좌표 평면과 평행을 유지해야 하므로 물체의 회전은 AABB를 회전시키지 않고
    AABB의 면적을 변경한다.
    일부 게임에서는 AABB 회전을 계싼할 필요가 없을 수도 있다.
    게임상의 대부분의 인간형 캐릭터는 오직 상향축만을 기준으로 회전한다.
    즉, 캐틱터의 AABB를 충분히 넓게 만들면 캐릭터가 회전해도 변경할 필요가 없다.
    (단, 캐릭터를 너무 많이 움직이는 애니메이션에 대해선 주의한다)
    그러나 다른 오브젝트에는 회전에 대한 계산이 필요하다.

    오브젝트 회전 후 AABB 계산 방법 중 하나는 먼저 AABB의 구석을 나타내는 8개의 점을 생성하는 것이다.
    이 점들은 단순히 min, max 요소로 표현한 점들의 나열이다.
    그 후 오브젝트의 각 점을 회전시킨 뒤 회전된 점으로부터 새로운 AABB를 생성하기 위해 UpdateMinMAx를 사용한다.
    이 과정은 회전 후에 해당 오브젝트의 가장 작은 AABB는 계산하지 않는다.
    그러므로 게임은 여러 번 의 회전 후에 에러 전파를 하지 않도록 원래 물체의 AABB를 저장해야 한다.

    방향성 있는 바운딩 박스(OBB, Oriented Bounding Boxes)

    OBB는 ABB처럼 축이나 평면에 대해 평행해야 한다는 제한이 없다.
    그렇기에 물체가 회전해도 OBB는 해당 물체에 딱맞는 경계값을 유지한다.
    OBB를 표현하는 한가지 방법은 중심점과 회전을 위한 쿼터니언, 
    그리고 박스를 위한 값들(너비, 높이, 깊이)을 가지는 것이다.

    비록 OBB를 사용하는 게 매력적으로 보이지만, OBB의 단점은 충돌 계상량이 AABB보다 너무 비싸다는 것이다.

    캡슐(capsule)

    캡슐은 반지름을 가진 선분이다.

    struct Capsule
    {
        LineSegment mSegment;
        float mRadius;
    };

    캡슐은 주로 인간형 캐릭터를 나타내는 데 자주 사용된다.
    캡슐은 또한 구체가 움직인 영역이다.
    구체가 이동을 할 때는 시작점과 끝점이있고, 이동 중의 구체는 반경을 가지고 있기 때문이다.

    볼록 다가형(Convex polygon)

    때론 게임은 기본 형태보다 더 정확한 물체의 경계를 필요로 한다.
    2D 게임에서는 물체의 바운딩볼륨을 볼록 다각형으로 표현하는 것이 가능하다.
    내부의 모든 각이 180도보다 작다면 그 다각형은 볼록 다각형이다.

    볼록 다각형은 vertex의 collection으로 표현 가능하다.

    struct ConvexPolygon
    {
        // vertex는 시계 방향으로 정렬돼 있다.
        std::vector<Vector2d> mVertices;
    }

    이 vertex들은 다각형의모서리를 따라 시계 방향순, 또는 반시계 방향순으로 정렬돼야 한다.
    정렬이 안 돼 있다면 오브젝트 간 교차 확인은 계산하기 매우 어렵다.

    다각형이 볼록다각형이고 시계 방향 순으로 vertex가 정렬돼 있는지 검증하는
    테스트가 없으므로 볼록다각형구조체는 개발자가 올바르게 사용한다고 가정한다.

*/