/*
    스플라인 카메라(spline camera)

    스플라인은 곡선상의 일련의 점들로 구성된 곡선을 수학적으로 표현한 것이다.
    오브젝트가 일정 기간동안 곡선을 따라 부드럽게 움직일 수 있게 해주기에 자주 사용된다.
    카메라가 미리 정의된 스플라인 경로를 따라갈 수 있으므로 컷신(cutscene) 카메라에
    매우 유용하다.
    지금 프로젝트에선 카메라가 설정된 경로를 따라 이동한다.

    캣멀롬(Catmull-Rom) 스플라인은 상대적으로 계산하기 간단한 스플라인 타입이라
    게임과 그래픽스에서 자주 사용된다.
    P0에서 P3까지 4개의 제어점을 요구한다.
    실제 공선은 P1에서 P2까지이며, P0은 곡선이 시작되기 전의 제어점에 해당하며,
    P3는 곡선이 끝난 후의 제어점에 해당한다.
    최상의 결과를 얻으려면 이러한 제어점을 곡선상에 균등하게 배치해야 한다.
    균등한 배치를 위해 유클리드 거리를 사용한다.

    4가지 제어점이 주어지면 다음 매개변수 방정식처럼 P1, P2 사이의 점을 표현할 수 있다.
    t = 0일 떈 P1이며 t = 1일때는 P2이다.

    p(t) = 0.5 * (2 * P1 + (-P0 + P2) * t + (2 * P0 - 5 * P1 + 4 * P2 - P3) * t^2
            + (-P0 + 3 * P1 - 3 * P2 + P3) * t^3)

    캣멀롬 스플라인 방정식은 오직 4개의 제어점만 있지만,
    임의의 수의 제어점으로 스플라인을 확장하는 것도 가능하다.
    이 확장된 스플라인은 제어점들이 경로점이 되거나 아닐 수 있으며,
    경로 앞에 한 점 그리고 경로 뒤에 한 점이 여전히 존재하므로 잘 동작한다.

    n 점의 곡선을 나타내는 데는 n+2개의점이 필요하며 개발자는 4개의 인접한 점을
    얻은 뒤 이 4개의 점들로 구성된
    스플라인 방정식을 사용하면 된다.

    스플라인 경로를 따르는 카메라 구현시 먼저 스플라인을 정의하는 구조체를 정의해야 한다.
    Spline 구조체가 필요한 유일한 멤버 데이터는 제어점을 담는 벡터다.

    Compute 함수도 작성하는데 P1에 해당하는 시작 인덱스와 [0.0, 1.0] 범위에
    있는 t값을 스플라인 방정식에 적용한다.
    또한 이 함수는 startIdx가 유효한지 검증하기 위한 경곗값 확인을 한다.

    SplineCamera 클래스는 멤버 데이터로 Spline 구조체, P1에 해당하는 현재 인덱스,
    현재 t값, 속도, 카메라가 경로를 따라 이동해야 하는지 여부를 결정하는 플래그값을
    정의한다.

    스플라인 카메라는 먼저 속도와 델타 시간의 함수로 t값을 증가시켜 갱신한다

    t값이 1.0보다 같거나 크면 P1은 경로상의 다음점으로 이동한다.(충분한 점이 있다고 가정)
    P1이 이동하는 건 t값에서 1을 빼야 한다는 걸 의미한다.
    스플라인에 더 이상 점이 없다면 카메라는 멈춘다.

    카메라 계산에서 카메라 위치는 스플라인으로부터 계산된 점이다.
    대상 지점을 계산하려면 스플라인 카메라가 이동하는 방향을 결정하기 위해 작은
    델타값으로 t를 증가시켜야 한다.

    마지막으로 상향 벡터는 스플라인이 거꾸로뒤집혀지지 않는다는 것을 가정해 (0, 0, 1)로
    유지하나 변경할 수도 있을것같다.
*/
