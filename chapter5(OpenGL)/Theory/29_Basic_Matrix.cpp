/*
    행렬과 변화(Matrix and Transformation)

    행렬은 m행과 n열이 있는 값의 격자다.
    
    그래픽스 분야에서는 행렬을 사용해 변환을 표현한다.
    행렬이 선형 방정식 시스템의 문제를 푸는 데 사용된다.
    따라서 방정식 체계를 행렬로 표현할 수도 있따.

    행렬 곱셈

    스칼라와 마찬가지로 두 행렬은 서로 곱할 수있따.

    A = | a b |,    B = | e f |
        | c d |         | g h |

    두 행렬이 잇을 때 곱셈 C = AB

    C = | a * e + b * g     a * f + b * h |
        | c * e + d * g     c * f + d * h |

    즉 C의 왼쪽 위 요소는 A의 첫 번째 행과 B의 첫 번째 열과의 내적이다.

    행렬 곱셈은 같은 차원을 가진 행렬을 요구하지 않는다.
    하지만 왼쪽 행렬의 열의 수는 오른쪽 행렬의 행의 수와 같아야한다.

    | a b | * | c d | = | a * c + d * e     a * d + b * f |
              | e f | 

    행렬 곱셈은 결합은 가능하지만 교환은 가능하지 않다.

    행렬을 사용한 점의 이동

    행렬은 임의의 점으로 표현하는 것이 가능하다.
    점 p = (x, y)는 하나의 행(row vector)로 나타낼 수 있다.

    p = | x y |

    또한 하나의 열(column vector)로도 나타낼 수 있따.

    p = | x |
        | y |

    둘 다 상관없지만 일관된 하나의 방식을 사용하는 것이 중요하다.
    점이 행인지 열인지에 따라 벡터가 곱셈의 왼쪽 또는 오른쪽에 위치될지가 결정되기 때문이다.

    변환 행렬 T가 있다고 가정한다.

    T = | a b |
        | c d |

    행렬 곱셈을 사용하면 이 행렬로 점 P를 변환해서 변환된 점 (x', y')을 얻을 수 있다.
    하지만 p가 행일 떄 T를 곱한 결과와 P가 열일 때의 결과가 서로 다르다.

    P가 행일 때

    | x' y' | = pT = | x y | * | a b |
                               | c d |
    x' = a * x + c * y
    y' = b * x + d * y

    하지만 p가 열이라면

    | x' |  = TP = | a b | * | x |
    | y' |         | c d |   | y |
    x' = a * x + b * y
    y' = c * x + d * y

    이렇게 2가지 다른 값을 돌려주지만 하나만이 올바른 해답이다.

    두 방법중 사용 여부는 가제돼 있지 않다.
    대부분의 선형 대수 교과서에서는 열벡터를 사용한다.
    하지만 그래픽스에서는 리소스나 그래픽 API에 따라 행 벡터 또는 열 벡터를 사용한다.
    이 책, 공부,에서는 행 벡터를 사용하는데 주 이유는 변환이 주어진 점에 대해
    왼쪽에서 오른쪽순으로 적용되기 때문이다.

    다음 방정식은 점 q를 먼저 행렬 T로 변환한다.
    그리고 행렬 R로 변환시킨다.

    q' = qTR

    각 변환 행렬에 트랜스포즈(transpose)를 적용하면 행 벡터를 열 벡터로, 열 벡터로르 행 벡터로 전화하는 것이 가능하다.
    행렬의 트랜스포즈는 원래 행렬의 첫 번째 행이 결과 행렬의 첫 번째 열이 되도록 행렬을 회전시킨다.

    | a b |^T = | a c |
    | c d |     | b d |

    점 q를 열 벡터로 사용한 방정식으로 전환하길 원한다면 다음과 같이 계산한다.
    q' = R^T * T^T * q

    이번 공부에선 행 벡터를 사용한다고 가정한다.
    하지만 행렬의 트랜스포즈는 열 벡터로도 작업 가능하게 변환된다.

    항등행렬(Identity matrix)가 있다.

    항등 행렬은 대문자 I로 표시되는 특별한 유형의 행렬이다.
    이 행렬은 같은 수의 행과 열을 가진다.
    모든 값은 댁가선을 제외하고 0이다.
    대각선 값은 모두 1이다.
    3x3 항등 행렬 I
        | 1 0 0 |
    I = | 0 1 0 |
        | 0 0 1 |

    항등 행렬을 임의의 행렬과 곱하면 행렬은 변견되지 않는다.

    MI = M
*/