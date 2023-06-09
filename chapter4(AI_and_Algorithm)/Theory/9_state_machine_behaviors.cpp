/*
    상태 기계 행위(State Machine Behaviors) (AIComponent 클래스)

    행위의 변화를 표현하는 한 가지 방법으로 각 행위가 하나의 상태를 가지는 상태 기계(State Machine)이란 것이다.

    ex) 팩맨에서 각 유령은 3가지 다른 행위를 가진다.
        1. 플레이어 쫓아가기
        2. 흩어지기(유령이 '홈 지역' 세트로 돌아오기)
        3. 플레이어로부터 멀어지기

    상태 기계 설계하기

    상태 그 자체는 부분적으로 하나의 상태 기계만 정의한다.
    그래서 설계는 상태 기계를 변경하거나 상태 기께 간 전이(transition)하는 방법을 결정하는 것이 중요하다.
    또한 각 상태는 해당 상태에 진입하거나 빠져나갈 때 특정 행위를 수행한다.

    서로 다른 상태가 어떻게 상호 연결되는지 계획하는 게 좋다.
    ex) 스텔스 게임에서 기본 가드(guard) 캐릭터의 상태 기계
        3가지 유형의 상태
        1. 순찰(Patrol) - 기본적으로 정해진 경로를 순찰
        2. 공격(Attack) - 플레이어를 감지한 경우
        3. 사망(Death) - 치명적인 데미지를 받은 경우
        
    이렇게 단순한 상태 기계에서 랜덤한 상태를 추가하거나 더 다양한 상황에 대한
    상태를 설계하면 복잡한 상태 기계를 만들 수 있따.

    구현하는 방법은 몇 가지 방법이 존재한다.
    코드는 현재 상태를 기반으로 AI의 행위를 갱신한다.
    그리고 상태 기계는 상태 진입 및 상태 종료 시의 Action을 지원해야 한다.

    코드로 각 상태를 나타내는 방법은 클래스를 사용해 나타낸는 것이 좋다.(AIState 클래스)
    그러면 각 상태가 별도의 서브클래스에서 구현됐으므로 AIComponent는 간결함을 유지할 수 있다.
    여러 다양한 AI 캐릭터가 같은 상태를재사용하기 쉽다.

    새 Actor의 AIComponent에 원하는 상태를 등록만 해주면 된다.

*/