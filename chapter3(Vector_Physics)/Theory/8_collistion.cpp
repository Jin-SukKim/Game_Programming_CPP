/*
    기본 충돌 감지(Collision detection) - (CircleComponent 클래스)

    충돌 감지는 두 물체가 서로 접촉하는지 결정하는 방법이다.
    핵심 개념은 문제의 단순화다.

    오브젝트의 이미지가 원형태일 때 정확하게 원은 아니다.
    실제 오브젝트의 윤곽에 대해 충돌을 테스트하는 것이 보다 정확하겠지만,
    충돌 감지란 목적에서는 오브젝트를 단순한 원과 같은 도형으로 생각하는게 효율적이다.
    (운석과 레이저일 경우 둘 다 원으로 단순화 하면 두 원이 충돌하는지만 결정하면 된다.)

    원과 원의 교차

    두 원은 두 원 중심점 사이의 거리가 두 원 반지름 합보다 같거나 작은 경우에만 서로 교차한다.

    세가지 경우

    1. 중심점 사이의 거리가 두 반지름의 합보다 큰 경우
        - 충분히 멀어 두 원이 교차하지 않는다.

    2. 두 중심점  사이의 거리가 두 원 반지름의 합보다 작은 경우
        - 원이 교차한다.

    교차 테스트를 위해 두 원 중심점 사이의 벡터를 만들고,
    벡터의 크기를 계산한 후 각 반지름의 합과 비교한다.

    ||A.center - B.center|| <= A.radius + B.radius, 원 A, B

    sqrt계산은 값이 크므로 제곱값으로 비교해도 된다.

    하지만 원은 모든 유형의 물체에 잘 동작하지 않으며 3D에서 특히 잘 동작하지 않는다.

    나중에 더 깊이 공부해본다.
*/