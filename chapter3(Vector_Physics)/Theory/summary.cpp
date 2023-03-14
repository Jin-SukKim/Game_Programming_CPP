/*
    벡터(vector)는 크기와 방향을 나타낸다.
    
    벡터를 사용하면 두 점 사이의 벡터 생성(벡터의 뺄셈),
    두 점 사잉의 거리 계산(뺄셈과 벡터의 길이 사용),
    두 벡터 사이의 각도 찾기(벡터의 내적),
    그리고 표면의 법선 벡터 계산(벡터의 외적)을 포함한 다양한 연산이 가능하다.

    기본 이동을 위해 Actor가 회전하고 전방으로 이동 가능하게 해주는
    (MoveComponent)클래스를 제작했었다.

    앞으로 이동하려면 Actor의 전방 벡터에 이동 속도와 델타 시간을 곱해서
    구한 벡터를 Actor의 현재 위치에 더해야 한다. (Actor는 새로운 좌표를 얻는다.)

    Actor나 Component로 입력을 전달하고자 InputComponent 클래스를 제작했다.

    뉴턴 물리(Physics)에서 물체의 가속도는 물체의 가해진 힘을 물체의 질량으로 나눈 값이다.
    오일러 적분을 사용하면 프레임마다 속도나 위치의 변화를 계산할 수 있다.

    충돌감지(collision)은 두 물체가 서로 교차하는지를 결정하는 방법이다.
    일부 유형은 원을 사용해 충돌을 판단한다.
    두 원은 두 원 중심점 사이의 거리가 두 원 반지름의 합보다 작으면
    교차하는 것으로 간주한다
    두 값의 비교결과는제곱한 값으로 비교해도 동일하므로 최적화를 위해 제곱한 값으로 비교한다.

    연습

    1. 벡터 a = (2, 4), b = (3, 5), 스칼라 s = 2가 주어졌을떄 다음을 계산하자
        a) a + b
        b) s * a
        c) a * b

    2. 삼각형은 아래의 세 점 A, B, C로 그려졌다
        A = (-1, 1)
        B = (2, 4)
        C = (3, 3)
        벡터 연산을 사용해 A각도를 계산하자

    3. 타깃으로 향하는 화살표를 가진 플레이어가 있다고 가정할 때,
        게임을 처음 시작할 때 화살표는 x축 (1, 0)을 가리킨다.
        플레이어의 초기 위치는 (4, 0)이다.
        이제 (5, 6) 위치에 새 물체를 생성한다.

        a) 플레이어의 초기 위치에서 새로운 목표물까지의 단위 벡터는 무엇인가?
        b) 초기 화살표 방향과 a 에서 계산된 벡터 사이의 회전 각도를 계산하라
        c) 초기 화살표 방향과 a 에서 계싼된 벡터로 생성된 평면과수직한 벡터를 계산하라

    4. 프로젝트의 운석과 우주선이 충동하도록 구현해본다.
        우주선에 CollisionComponent를 만들고 반지름을 지정하고
        Ship::UpdateActor에서 모든 운석과의 충돌을 테스트한다.
        우주선이 운석과 충돌하면 우주선의 위치를 화면의 중심으로 초기화하고 회전값은 0으로 설정한다.

        추가 기능으로 우주선이 운석과 충돌한 후 1~2초 정도 화면에 출력되지 않게 구현한다.

    5. 뉴턴 물리를 사용하도록 MoveComponent를 수정한다.
        멤버 변수로서 질량, 힘의 총합, 속도를 가지도록 변경하고
        Update함수에서 전방 이동에 대한 코드를 변경해서 힘으로부터 가속도를,
        가속도로부터 속도를, 속도로부터 위치를 계산한다.

        Componenet에 힘에 설정하는 메소드를 구현한다.
        한 가지 방법은 Vector2를 parameter로 받는 AddForce 함수를 추가하고
        힘의 총합 변수에 그 힘을 더하는 것이다.
        힘의 총합으로부터 가속도를 계산하고 난 다음, 힘의 총합을 초기화한다.
        그러면 충격력의 경우 AddForce를 한 번만 호출하면 된다.
        일정하게 작용하는 힘은 프레임마다 AddForce를 호출한다.

        마짐가으로 뉴턴 물리를 지원하는새로운 MoveComponent가 잘 동작하도록
        InputComponent, Asteroid, Ship을 수정한다.
    
    
*/