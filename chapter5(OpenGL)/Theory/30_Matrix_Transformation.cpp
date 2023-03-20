/*
    행렬로 transform

    행렬로 크기, 회전, 이동 변환을 표현하는 것이 가능하다.
    변환을 결합한 통합 방정식보단 행렬을 서로 곱해 결합된 변환 행렬을 얻어 사용하자

    2D 상의 변환(이 공부에선 행 행렬을 사용한다.)

    크기 행렬

    S(sx, sy) = | sx 0 |
                | 0 sy |

    회전 행렬

    R(각도) = |  cos각도  sin각도 |
              | -sin각도  cos각도 |

    이동 행렬

    T(a, b) = | 1 0 0 |
              | 0 1 0 |
              | a b 1 |

    2x2 행렬은 크기와 회전의 표현이 가능하지만 이동은 불가능 하다.
    3x3을 사용해 이동을 구현해 줘야한다.

    그러나 2d vector인 1x2 행렬은 3x3행렬과 곱할 수 없다
    그래서 추가 열을 더해 1x3 행 벡터로 만들어서 곱해줘야 한다.

    이를 위해 점에 추가 요소를 더하는데 이것을 동차(homogenous) 좌표라고 한다.
    동차 좌표는 n차원 공간을 나타내기 위해 n+1 요소를 사용한다.
    그래서 2D 공간에서 동차 좌표는 3개의 요소를 사용한다.

    3D 공간을 나타내려한 것이 아닌 특별한 요소를 더한 것을 w 요소라고 불른다.
    2D/3D에서 모두 w로 표현한다.
    (2D : (x, y, w), 3D : (x, y, z, w))

    지금은 w 요소에 값 1만 사용한다.

    점 p(x, y)는 동차 좌표 (x, y, 1)로 표현된다.

    이렇게 동촤 좌표 점을 변환 행렬과 곱해줘도
    x, y요소는 변화하고 w 요소는 여전히 1을 유지한다.

    변화 결합

    여러 변환 행렬은 서로 곱해서 결합하는 것이 가능하다.
    2x2행렬에 3x3행렬을 곱할 수 없으므로 크기와 회전 변환을 동차 좌표로 동작하는 3x3 행렬로 표현해준다.

    크기
    S(sx, sy) = | sx 0 0 |
                | 0 sy 0 |
                | 0  0 1 |

    회전
    R(각도) = |  cos각도  sin각도  0 |
              | -sin각도  cos각도  0 |
              |     0       0     1 |
    
    이동
    T(a, b) = | 1 0 0 |
              | 0 1 0 |
              | a b 1 |

    이렇게 모두 3x3 행렬 형태로 표현했으므로 이를 결합해 하나의 변환 행렬을 만들수 있다.
    오브젝트 공간에서 세계 공간으로 변환하는 이 결합 행렬은 세계 변환 행렬(world transform matrix)이다.

    worldtransform = S(sx, sy) * R(각도) * T(a,b)

    이 곱셈 순서는 변환을 적용하려는 순서와 동일하다.
    이렇게 얻은 행렬을 vertex shader에 전달하고 오브젝트의 모든 정점을 전달된 행렬로 변환시킨다.

*/