/*
    게임 트리(game tree) (GTNode structure) - GameTree.cpp

    틱택토나 체스같은 게임은 다른 실시간 게임과 매우 다르다.
    플레이어가 2명이며 각 플레이어는 차례를 번갈아가며 진행한다.
    또한 적대적인 상황에 놓여 있어 서로 대결하는 구조다.

    이런 유형의 게임의 AI 요구 사항은 실시간 게임의 요구 사항과는 매우 다르다.
    게임은 게임 상태를 유지하는 것이 필요하며, 이상태는 AI의 의사결정에 영향을 미친다.
    게임 상태를 유지하는 한 가지 접근법은 게임 트리(game tree)란 트리 구조를 사용하는 것이다.

    게임 트리에서 류ㅜ트 노드는 게임의 현재 상태를 나타낸다.
    각각의 edge는 게임에서 가능한 이동을 나타내며 각 이동은 새로운 게임 상태와 연결된다.

    ex) 틱택토 게임

        루트 노드로부터 시작해 현재 플레이어(Max player라고 불림)은 3가지 이동의 선택이 가능하다.
        맥스 플레이어가 이동 후 게임 상태는 트리으 ㅣ첫 번째 레벨에 있는 노드로 전이한다.
        그럼 적(Min Player)는 트리의 두 번째 레벨로 이끄는 이동을 선택한다.
        이 과정을 종료 상태를 나타내는 리프 노드(leaf node)에 도달할 때까지 반복한다.
        리프 노드에 할당된 숫자 값은 승리, 패배, 무승부 3가지 결과를 나타낸다.
        (맥스 플레이어 관점이다)

        여러 게임들은 다양한 상태를 가진다.
        틱택토 게임은 2D 배열 형식의 보드로 표현하면 된다.

    게임 트리 노드는 해당 노드의 게임 상태뿐만 아닌 자식 노드 리스트도 저장한다.

    환전한 게임 트리를 생성하려면 루트 노드를 현재 게임의 상태로 설정하고 이동 가능한
    자식 노드를 생성해야 한다.
    그런 다음 첫 번째 레벨의 각 노드에 대해서도 이 과정을 반복하고 모든 이동이 완료될때까지 진행한다.
    
    게임 트리의 크기는 잠재적인 이동의 수에 따라 지수적으로 커진다.
    틱택토 게임의 트리 상항선은 9!(362,880)개의 노드가 필요하다.
    하지만 체스는 10^120!의 노드를 가지므로 시간과 공간의 복잡도를 완전히 평가하는 것이 불가능하다.
    지금은 완전한 게임 트리가 있다고 가정하에 있지만 불완전 트리를 다루는 방법도 있다.
*/