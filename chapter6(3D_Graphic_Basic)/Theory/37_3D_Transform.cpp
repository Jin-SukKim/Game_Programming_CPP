/*
    3D에서 Actor 변환(Transform)

    2D 게임에선 x가 수평, y가 수직이었다.
    하지만 +y가 위나 아래일 수 있으니 좌표 체계는 구현 방법에 의존하는 것이다.
    세번째요소를 더하면 가능한 표현 방법이 증가한다.

    이번 공부에서는 +x는 앞으로, +y는 오른쪽으로, +z는 위쪽으로 표현한다.
    즉, x는 앞뒤, y는 좌우, z는 위아래이다.

    왼손을 사용해 엄지가 위쪽으로 향하게 하면 검지는 앞으로 향하고 중지는 오른쪽으로
    향하는데 이런 타입의 좌표 시스템을 왼손 좌표 시스템(left-handed cooridnated system)이라 한다.
    +y가왼쪽을 향한다면 오른손 좌표 시스템이 될 것이다.
    (언리얼 엔진이 왼손 좌표계를 사용한다)

    3D Tranformation 행렬

    3D 좌표를 사용하는 것은 동차 좌표가 존재함을 뜻한다(x,y,z,w)
    3D 좌표에서 변환 행렬은 4x4 행렬이 된다.

    4x4 이동 행렬은 오프셋 (a, b, c)로 이동시킨다.
    이동
    T(a, b) = | 1 0 0 0 |
              | 0 1 0 0 |
              | 0 0 1 0 |
              | a b c 1 |

    크기 행렬은 세 요소를 스케일 한다.          
    크기
    S(sx, sy) = | sx 0  0 0 |
                | 0 sy  0 0 |
                | 0  0 sz 0 |
                | 0  0  0 1 |

    하지만 3D에서 회전은 간단하지 않다.
    3D에서는 세 좌표축 전부에서도 회전하는 것이 유효하다.
    회전에 대한 접근 방식 중 하나로 오일러 각(Euler angle)이 있다.

*/