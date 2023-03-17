// minimax 알고리즘 
/*  
    (GameTree.cpp)
    이 알고리즘은 2인용 게임 트리를 평가해서 현재 플레이어에 대한 최선의 이동을 결정한다.
    미니맥스는 각 플레이어가 자신에게 가장 최선인 선택을 한다고 가정한다.
    점수는 맥스 플레이어의 관점이다.

    그래서 맥스 플레이어는 자신의 점수를 극대화하려고 하고
    민 플레이어는 점수를 최소화하려고 한다.

    미니맥스의 구현을 위해 민 플레이어와 맥스 플레이어의 행동에 대해
    별도의 함수를 사용한다.
    두 함수 모두 최초에는 노드가 리프 노드인지를 시험한다.
    리프 노드인 경우 GetScore 함수를 호출해 점수를 계산한다.
    두 함수는 재귀 호출을 사용해 최선의 하위 트리를 결정한다.

    맥스 플레이어의 경우 최선의 하위 트리는 최고의 값을 산출하는 트리다.
    반대로 민 플레이어는 가장 낮은 값을 가진 하위 트리를 찾는다.

    루트 노드에서 MaxPlayer를 호출하면 최선의 점수를 반환한다.
    하지만 AI 플레이어가알고 싶은 최적화된 다음 이동을 지정하지 않는다.

    최선의 이동을 경절하는 함수를 따로 만들어준다. (MinimaxDecide 함수)
    어떤 자식 노드가 최선의 값을 산출하는지를 추적한다.

*/