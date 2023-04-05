/*
    선분 테스트

    선분은 충돌 감지에서 다양하게활용된다
    ex) 공 발사체가 오브젝트와 충돌하는지를 테스트하기 위해 사용되기도 한다.

    선분과 다른 오브젝트 간 몇 가지 핵심 테스트를 공부한다.
    이 테스트는 선분이 교차하는지 여부뿐만 아닌 최초로 교차된느 점을 알아내는 것도 포함한다.

    선분 매개 방정식을 사용한다.

    L(t) = Start + (End - Start) * t, where 0 <= t <= 1

    대부분의 선분 교차 테스트접근법은 선분을 무한히 긴 서으로 다룬다.
    무한한 선이 오브젝트와 교차하지 않으면 당연히 선분도 물체와교차하지 않기 때문이다.
    무한히 긴 선과 오브젝트가 교차한다면 t ㄱ밧이 [0, 1] 경계 내의 값인지 확인해 
    선분과 오브젝트의 충돌을 최종 검증한다.

    선분과 평면 교차 테스트

    선분과 평면의 교차점을 찾기 위해 점 L(t)가 평면상에 위치하게 하는 t를 찾는다.

    L(t) * n  + d = 0, L(t) * n = 내적

    몇가지 대수 계산을 수행해 t를 구한다.
    먼저 L(t)를 치환한다.

    (Start + (End - Start) * t) * n + d = 0

    내적은 덧셈에 대해 분배법칙이 가능하다.

    Start * n + (End - Start) * n * t + d = 0

    식을 t에 대한 식으로 정리한다.

    (End - Start) * n * t = -Start * n - d
    t = (-Start * n - d) / ((End - Start) * n), n은 내적

    분모의 내적이 0이 되면 오류가 나므로 주의한다.
    이 경우 선이 평면의 법선에 수직인 경우메나 발생하며 선이 평면과 평행하다는 것을 뜻한다.
    이 경우 선이 평면상에 있을 경우에만 교차한다.

    t값을 계산한 후, t가 선분의 경계 내에 있는지를 테스트한다.
    Intersect 함수는 참조로 t값을 반환한다.
    호출자는 선분과 평면의 교차점이 필요할 시 이 t값을 활용한다.

    선분과 구체의 교차 테스트

    선분과 구 사이의 교차점을 찾기 위해 우선 선과 구체 C사이의 거리가 구의 반지름 r과 같은 t가 있는지를 찾는다.
    
    ||L(t) - C|| = r
    ||Start + (End - Start)t - C|| = r
    ||Start - C + (End - Start)t|| = r

    이 방정식을 간단히 하기 위해 변수를 치환한다.

    X = Start - C
    Y = End - Start
    ||X + Yt|| = r

    t를 구하려면 길이 연산 내부에서 t를 끄집어낼 방법이 필요하다.
    이를 위해 방정식양쪽으로 제곱하고 길이의 제곱을 내적 형태로 대체한다.

    ||X + Yt||^2 = r^2
    (X + Yt) * (X + Yt) = r^2

    내적은 벡터 덧셈에 대해 분배 법칙이 가능하므로 FOIL(first, outside, inside, last) 분배법칙을 적용한다.

    X * X + 2X * Yt + Y * Yt^2 = r^2

    이차방정식 형태로 정리한다.

    Y*Yt^2 + 2X*Yt + X*X - r^2 = 0
    a = Y * Y
    b = 2X * Y
    c = X * X - r^2
    at^2 + bt + c = 0

    t에 관한 이차방정식 형태로 정리

    t = (-b +- sqrt(b^2 - 4ac)) / 2a

    이 이차방정식의 판별식(특히 제곱근 기호 아래 값)은 방정식의 해의 개수와 유형을 알려준다.
    판별식이 음수라면 해는 허수다.
    게임에는 목정상 물체의 허수 위치가 존재하지 않는다.
    즉, 음수 판별식은 선이 구체와 교차하지 않는다는 걸 의미한다.

    판별식이 0이라면 선이 구에 접하므로 1개의 해가 있다는 걸 의미한다.
    판별식이 0보다 크면 구체와교차하는 점이 2개 있다.

    t의 해를 구하면 t가 [0, 1] 범위 내부의 값인지를 검증한다.
    t는 2개의 핵 ㅏ존재할 수 있기에 최초의 교차를 나타내는 작은 t값을 선택한다.
    하지만 선분이 구의 내부에서 시작한다면 큰 t값이 교차점이다.
    만약 구체가 선분 전체를 초함하면falseㄹ르 반환한다.


    선분과 AABB 테스트

    선분과 AABB 교차 테스트 방법 중 하나는 박스의 각 가장자리에 평면ㅇ르 만드는 것이다.
    2D에서는 4개의 가장자리에 4개의 평면을 만들 수 있다.
    평면은 무한하므로 가장자리 평면과 선분이 교차한다고 해서 선분이 박스와 교차한다는 걸
    의미하지는 않는다.

    1. 선분 P1이 위쪽 모서리평면과 교차하고 왼쪽 모서리 평면은 P2와 교차한다.
        박스는 이 점들을 폼하고 있지 않으므로 이 선분은 박스와 교차하지 않는다.
    2. 선분이 왼쪽 모서리 평면 P3와 교차하는데 박스가 P3를 초함하므로 교차점이다.
    3. 복수 교차점을 가질 수 있다.
        선분이 박스를 지나가 P4, P5 교차점이 존재한다.
        이 경우 교차는 시작점과 가까운 점을 반환하거나 선분 매개 방정식에서
        작은 t값을 가지는 점을 반환한다.

    선분과 평면 교차에 대한 방정식

    t = (-Start*n - d) / ((End - Start)*n)

    각각의 평면은 좌쵸푹과 평행하므로(3D에서는 좌표 평면에) 각 평면의 법선은 쉽게 구할 수 있다.
    특히 3D의 경우, 2개의 요소는 항상 0이고 한 요소만이 1이므로 위의 식은 간단하게 최적화된다.
    즉 3개의 내적 요소 중 2개는 항상 0이라는 뜻이다.

    예를 들어 2D 상에서 왼쪽모서리 평면의 법선은 왼쪽이나 오른쪽을 가리킨다.
    2D에서 방향은 교차에 한해서는 중요하지 않다.
    그래서 법선 벡터는 다음과 같다.

    n = (1, 0)

    박스의 min 점은 왼쪽 모서리 평면에 있으므로 d값은 다음과 같다.

    d = -P*n = -min*(1, 0) = -minx

    마찬가지로 선분과 평면의 교차 방정식에서 내적은 x요소만으로 간소화된다.
    즉, 왼쪽 모서리 평면에 대한 교차점의 해를 구하는 최종 방정식은 다음과 같다.

    t = (-Start * (1, 0) - d) / ((End - Start) * (1, 0)) = (-Startx - (-minx)) / (Endx - Startx)
      = (-Startx + minx) / (Endx - Startx)

    다른 평면의 방정식도 비슷하게 유도된다.
    3D에서는 테스트해야 될 면이 총 6개이다.
    하나의 평면을 테스트하기 위한 작업을 캡슐화한 헬퍼 함수 TestSidePlane을 만등다.
    이 함수는 선분이 평면과 교차하면 t값을 std::vector에 추가한다.
    교차 함수는 이 std::vector를 사용해모든 가능한 t값을 순서대로 테스트한다.

    Intersect 함수는 선분과 AABB 6개 평면과의 교차 여부를 시험하기 위해 TestSidePlane함수를 사용한다.
    Intersect 함수는 ㅍ여면과 교차하는 각각의 점 t를 tValues 벡터에 저장하고,
    오름차순으로 이 벡터를 정렬한 뒤 AABB에 초함되는 최초의 교차점을 반환한다.
    교차점이 없다면 false를 return한다.

    박스의각 측면을 독립적으로 테스트하므로 어떤 평면이 선분과 교차하는지를 반환하도록
    교차 함수를 수정하는 것이 가능하다.
    이렇게 수정하면 물체가 상자 바깥으로 튀어나와야 하는 경우에 유용하다.
    이 작업을 구현하려면 TestSidePlane 각각의 호출을 박스의 측면과 연관지어야 한다.
    그런 다음 교차한 측면(또는 측면의 법선)을 Intersect 함수가 얻도록 참조파라미터를 추가하면 된다.

    두 평면으로 묶인 무한한 평판(slabs)을 사용하면 선분과 AABB 교차는 좀 더 최적화가 가능해진다.
    이 접근법을 사용하려면 추가적인 수학 배경 지식이 필요하다.

*/