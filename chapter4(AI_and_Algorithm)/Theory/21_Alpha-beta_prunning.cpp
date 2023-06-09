/*
    알파 베타 가지치기(Alpha-beta prunning)

    이것은 평가하려는 노드의 수를 줄이는 미니맥스 알고리즘의 최적화 기법이다.
    알파 베타 가지치기는 계산 시간을 증가시키지 않고 탐색하려는 최대 깊이를 증가시키는 것이 가능하다.

    알파와 베타라는 2개의 변수를 필요로 한다.
    알파는 현재 레벨이나 그 이상에서 맥스플레이어에게 보장하는 최고 점수다.
    반대로 베타는 현재 레벨이나 그 이상에서 민 플레이어에게 보장하는 최고 점수다.
    즉, 알파와 베타는 점수의 상한과 하한 경곗값이다.

    (AlphaBetaDecide function)
    처음에 알파는 음의 무한대 값이고, 베타는 양의 무한대 ㄱ밧이다.
    두 플레이어는 최악의 점수로 시작한다.

    (AlphaBetaMax function)
    Max Player는 어떤 반복에서도 최댓값이 베타값보다 같거나 크다면 이전 베타값은 더 좋아질 수 없다.
    이 상황에서는 함수가 남아 있는 형제 노드를 확인할 필요가 없기에 결과를 반환하고 종료한다.
    더 이상 조사하지 않는 것이다.
    그러지 않고 최댓값이 베타값보다 작고 알파값보다 크다면 코드는 알파값을 증가시킨다.

    (AlphaBetaMin function0)
    Min Player는 최솟값이 알파와 같거나 더 작은지를 검사한다.
    이 경우 이전 알파값이 더 좋아질 수 없으므로 함수는 종료된다.
    아니면 코드는 상황에 따라 베타값을 감소시킨다.

    자식 노드에 대한 평가 순서는 내쳐지는 노드 수에 영향을 미친다.
    이는 일관된 깊이 제한을 가진다 하더라도, 시작 상태에 따라 실행 시간이 달라질 수 있음을 뜻한다.
    이런 상환은 AI가 고정된 시간 제한을 가진다면 문제가 된다.
    불완전한 탐색은 AI가 어떤 이동을 해야 할지 결정 못했다는 것을 의미하기 때문이다.

    해결책은 깊이 제한ㄱ밧을 증가시키면서 알고리즘을 여러 번 실행하는
    반복적인 심화 방법이대안이 될 수 있다.
    (ex: 3의 깊이 제한그로 실행해 기준이 되는 이동 산출 -> 4의 깊이 제한 -> 5의 깊이제한)

    이런 반복에서 기본 이동을 구했기 때문에 특정 깊이에서 부뢍ㄴ전한 탐색을 했다 하더라도
    AI가 어떤 이동을 해야 할지 선택하는 데는 문제가 없다.
*/