/*
    팔로우 카메라(follow camera)

    이 카메라는 타깃 오브젝트 뒤를 따라가는 카메라다.
    이런 유형의 카메라는 차 뒤를 따라가는 레이싱 게임과 3인칭 액션/어드벤처 게임을 포함한
    수많은 게임에서 널리사용된다.
    팔로우 카메라는 다양한 게임에서 사용되므로 실제 구현도 다양하며 변형이 많다.
    지금은 차를 따라가는 카메라를 중점적으로 다룬다.

    1인칭 캐릭터와 마찬가지로 FollowActor 클래스를 만들어 팔로우 카메라 구현을 담당한다.
    이동은 W/S로 전/후진, A/D키로 왼/오른쪽 회전을 담당한다.
    MoveComponent는 두 타입의 이동을 지원하므로 수정이 필요없다.

    기본 팔로우 카메라

    기본은 항상 소유자 Actor의 뒤쪽과 위쪽으로 설정된 거리에서 소유자 actor를 따라간다.
    레이싱 게임에서 카메라는 차 뒤로 HDist 수평 거리만큼,
    차 위로 수직 거리 VDist 만큼 떨어진 거리에 배치된다.

    카메라의 타깃 지점은 차 그 자체는 아니고, 차 앞에 있는 점 TargetPos이다.
    이로 인해 카메라는 차 그 자체를 직접 보는게 아닌 차량 앞의 지점을 보게 된다.

    카메라 위치를 계산하기 위해 Vector 더하기 및 Scalar 곱을 사용한다.
    카메라 위치는 소유자 뒤로 HDist 단위이며, 위로 VDist 단위다.

    카메라 위치 방정식

    CamearaPos = OwnerPos - OwnerForward * HDist + OwnerUp * VDist
    OwnerForward = 소유자의 전방 벡터
    OwnerUp = 소유자의 상향 벡터

    TargetPos는 소유자의 앞으로 TargetDist 단위만큼 떨어져 있는 지점이다.

    TargetPos = OwnerPos + OwnerForward * TargetDist

    FollowCamera 클래스를 만들어 준다.
    수평 거리(mHorzDist)와 수직 거리(mVertDist), 대상 지점(mTargetDist)에 대한 변수를 가진다.
    앞의 방정식을 사용해 카메라 위치를 계산하는 함수를 만들고
    Update 함수에서 계산된 목표 지점과 카메라 위치를 사용해 뷰 행렬을 생성한다.

    이 기본 카메라는 게임 세계를 돌아다닐 때 차량을 성공적으로 추적하지만 매우 경직된 느낌을 준다.
    왜냐하면 카메라는 항상 대상으로부터 고정된 거리에 있으므로 속도감을 얻기가 어렵기 때문이다.
    또한 차가 방향을 바꿀 때 차가 방향을 바꾸는 것이 아닌 세계가 방향을 바꾸는 것처럼 보인다.
    그래서 기본 팔로우 카메라는 좋은 시작점이지 세련된 해결책은 아니다.

    속도감을 향상시키는 한 가지 간단한 변화는 카메라와 소유자 사이의
    수평 거리를 소유자의 속도 함수로 만드는 것이다.
    그래서 카메라에 탄력성을 추가한다.
    ex) 정지 상태 수평거리 350 -> 최대 속도 이동 수평 거리 500으로 증가
    하지만 여건치 방향을 바꿀때는 경직된 것처럼 보인다.

    스프링 추가

    카메라 위치는 방정식에서 구한 위치를 즉각 반영하지 않고 여러 프레임에 걸쳐 
    목표 위치로 도달하는 형태로 구현하는 것이 가능하다.
    이를 위해 카메라 위치를 이상적인 위치와 실제 위치로 분리한다.
    이상적인 위치는 기본 팔로우 카메라 방정식으로부터 얻은 위치이고
    실제 카메라 위치는 뷰 행렬이 사용하는 위치다.

    이상적인 카메라와 실제 카메라가 스프링으로 연결됬다고 생각한다.
    초기에 같은 위치에 있따 이상적인 카메라가 움직이면 스프링은 늘어나고
    실제 카메라 또한 움직이기 시작하지만, 느린 비율로 움직인다.
    최종적으로 스프링이 완전히 늘어나고, 실제 카메라는 이상적인 카메라만큼 빠르게 움직인다.
    이상적인 카메라가 멈추면 스프링은 정상 상태로 돌아간다.(다시 같은 위치)

    스프링 구현을 위해 스프링 상수(mSpringConstant) 변수로 스프링의 탄성을 나타내 값이 클수록 탄성이 크게한다.
    카메라의 실제 위치(mActualPos)와 속도(mVelocity)를 기록해야 하므로 2개의 벡터 멤버를 추가한다.

    Update에서 스프링 상수를 토대로 스프링 댐핑(spring dampening)을 계산한다.
    이상적인 카메라 위치는 ComputeCameraPos 함수의 위치값이다.
    그리고 실제 위치와 이상적인 위치의 차를 계산하고, 이 거리의 차와 이전 속도의
    감쇄값을 토대로 카메라의 가속도를 계산한다.
    챕터3에서 나온 오일러 적분을 이용해 카메라의 속도와 위치를 갱신한다.
    마지막으로 타깃 위치 계산은 동일하게 유지하며, 뷰 행렬 함수는 이상적인 위치가 아닌
    실제 위치를 사용해 생성한다.

    용수철에 작용하는 힘은 f = -kx다.
    x = 용수철이 늘어나느 길이.
    기계 진동에서 힘은 F = ma + cv이다.
    m = 질량, a = 가속도, c = 댐핑(감쇄) 계수, v = 속도
    두 식을 합해 a에 관한 식으로 정리하면
    a = (-kx-cv) / m
    하지만 지금은 카메라이므로 질량이 문제가 되지 않아 m을 1로 둔다.
    즉 가속도를 구하는 최종 공식이다.
    a = -kx-cv

    스프링 카메라의 장점은 소유자 오브젝트가 회전하면 카메라는 그 회전을 시간을 두고 따라잡는데 있따.
    이는 소유자 오브젝트가 회전할 때 소유자 오브젝트의측면이 보인다는 것을 의미하며,
    이 효과로 세계가 회전하는 것이 아닌 오브젝트가 회전하고 있다는 느낌을 준다.

    마지막으로 게임 시작 시 카메라가 올바르게 시작하도록 FollowActor가 최초 초기화될 때
    호출하는 SnapToIdeal 함수를 만든다.

*/
