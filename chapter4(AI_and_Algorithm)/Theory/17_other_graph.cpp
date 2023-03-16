/*
    기타 그래프 표현

    실시간 액션 게임에서 중립 캐릭터(NPC)는 대개 격자상의 사각형에서 사각형으로 이동하지 않는다.
    그래서 그래프로 세계를 표현하는 것이 더 복잡해진다.

    경로 노드와 네비게이션 메시를 사용한 2가지 대안적 접근법이 있따.

    경로 노드(Path Node, Waypoint 그래프)에서 디자이너는 게임 세계에서 AI가
    이동할 수 있는 위치에 경로 노드를 배치한다.
    이 결로 노드는 그래프상의 노드로 직접 변환된다.

    일반적으로 경로 노드 사이에 edge는 자동적으로 생성된다.
    알고리즘 동작방식
    1. 각 경로 노드에 대해 해당 노드와 해당 노드 근처에 있는
       노드 사이에 장애물이 있는지 여부를 시험한다.
    2. 각 노드 사이에 차단이 없다면 edge를 생성한다.

    선분 캐스트(Line segment cast)나 유사 충동 테스트가
    노드 사이에 차단이 있는지를 결정할 수 있다.(나중에 충동 감지에서 선분 캐스트 구현방법이 나온다)

    경로 노드 사용시 주된 결점은 AI가 노드 또는 edge의 위치로만 이동할 수 있다는 것이다.
    경로 노드가 삼각형을 형성해도 삼각형 내부가 유효한 위치라고 보장할 방법이 없다.
    길 찾기 알고리즘은 노드나 edge가 아닌 위치의 경우 장애물이
    존재할 가능성이 있으므로 유효하지 않다고 가정해야 한다.

    문제 해경을 위해 AI가 이동에 제약을 받지 않는 공간을 많이 주거나
    이동을 위해 많은 경로 노드를 제공해야 한다.
    하지만 첫번째는 AI의 이상행동이, 두번째는 비효율적이다.
    즉, 성능과 정확도 간 트레이드 오프가 필요하다.

    다른 게임에서는 내비게이션 메시(Navigation mesh)를 사용한다.

    이 접근법은 그래프상의 각 노드는 블록 다각형(convex polygon)에 해당하며
    인접 노드는 인접한 볼록 다각형이다.
    각 노드를 볼록 다각형으로 정의하면 몇 개의 볼록 다각형만으로도
    게임 세계의 전체 지역을 나타낼 수 있따.

    내비게이션 메시를 사용하면 AI는 볼록 다각형 내부의 어떠한 지점도
    안전하게 이동할 수 있다.
    즉 AI의 기동성이 향상된것이다.

    크기가 다른 여러 캐릭터의 이동에 대해 더욱더 효과적이다.
    (ex: 소와 닭이 있을 때 소가 못가는 구역을 작은 닭은 갈수있따.) \

    내비게이션 메시의 자동 생성은 디자이너가 AI 경로 지정에 따른 영향을 걱정할
    필요없이 레벨을 변경할 수 있기에 유용하다.
    아지만 매우 복잡하므로 내비게이션 메시 생성을 구현한 오픈 소스 라이브러리를 사용하다.
    
    가장 인기있는 라이브러리 Recast는 3D 레벨의 삼각형 기하로 구성된 내비게이션 메시를 생성한다.
*/