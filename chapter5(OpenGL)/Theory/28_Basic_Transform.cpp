/*
    변환 기초(Transform)

    모델 공간의 오브젝트를 월드 공간으로 옮길때 좌표 공간을 변환하는 방법.

    좌표 공간을 변환할 때는 두 좌표 공간 사이에 기저 벡터가 같은지 또는
    다른지를 알아야 한다.
    (ex: 오브젝트 공간에서 y+가 위인데 월드 공간에서 y+가 오른쪽이면 월점에서 (0,5)는 오른쪽 5번째에 놓인다.)

    일단 두 공간의 기저 벡터가 동일하다고 가정한다.
    게임이 2D이므로 오브젝트 공간과 세계 공간 모두 y가 위, x가 오른쪽이라 가정한다.
    (이 2D 좌표계와 SDL 좌표체계는 다른다, SDL은 +y가 아래 방향)

    오브젝트의 각 정점에 수학 연산을 적용하면 좌표를 변환할 수 있따.

    한 가지 방법으로 대수 방적식을 사용하는 것이다.
    (나중에 나올 해결책을 이해하는 징검다리 역할을 한다.)
    현재는 2D라 x,y만 사용하지만 3D에서 z 좌표를 추가하면 된다.

    이동(Translation)

    이동은 점을 변환시키거나 오프셋값으로 이동시킨다.

    점(x,y)가 주어졌을 때 다음 방정식으로 오프셋 (a,b)만큼 이동시킨다.
    x' = x + a
    y' = y + b

    삼각형의 모든 정점에 같은 이동을 적용하면 삼각형의 이동이 가능하다.

    스케일(scale)

    삼각형의 각 정점에 적용하면 크기가 커지거나 작아진다.

    균등 스케일(uniform scale)은 같은 스케일 s를 사용해 스케일한다.

    x' = x * s
    y' = y * s

    비균등 스케일(non-uniform scale) 각 요소에 대해 별도의 스케일(sx, sy)을 곱한다.

    x' = x * sx
    y' = y * sy

    회전(Rotation)

    단위 원은 점 (1,0)에서 시작한다.
    90도 또는 PI/2 radian 회전은 점 (0, 1)로의 반시계 방향 회전이며
    180도 PI radian회전은 점 (-1, 0)이 된다.
    이 횐전은 일반적인 단위 원 다어그램에서 z축을 그리지 않는다 하더라도
    기술적으로 z축에 대한 회전이다.

    sin과 cos를 사용하면 임의의 점(x,y)를 각도만큼 회전시키는 공식이 된다.

    x' = x * cos각도 - y * sin각도
    y' = x * sin각도 + y * cos각도

    각도는 반시계방향 회전을 나타낸다.
    원점을 기준으로 한 회전이다.

    변환을 결합

    각 변환을 독립적으로 적용하지만 보통 동일한 정점에 여러 변환을 동시에 적용한다.(계산 빈도가 줄어든다)
    이런 변환들을 적용 시 올바른 순서로 결합하는 것이 중요하다.

    오브젝트 공간에서 세계 공간으로의 변환에서는 항상 스케일(크기), 회전, 이동순으로 변환을 적용한다.
    스케일(sx, sy), 각도, 이동(a,b)
    
    x' = sx * x * cos각도 - sy * y * sin각도 + a
    y' = sx * x * sin각도 + sy * y * cos각도 + b

    이 작업은 오직 정점을 오브젝트 공간(model space)에서 세계 공간(world space)로 변환만 한것이다.
    세계 공간의 좌표는 장치 좌표에 대해 정규화돼 있지 않으므로 여전히 vertex shader에서는
    많은 변환을 적용해야 한다.
    추가적인 변환은 간단하지 않은데 여러 좌표 체계 사이의 기저 벡터가 다르기 때문이다.
    그래서 추가적인 변환을 하나의 방정식으로 결합해버리면 복잡성이 불필요하게 증가한다.

    이에 대한 해결책으로 각 요소에 대해 별도의 방정식을 사용하지 않고 대신
    행렬을 사용해 여러 변환을 표현한다.
    그래서 행렬 곱셈으로 여러 변환을 결합한다.

*/