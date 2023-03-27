/*
    버스(bus)

    FMOD 스튜디오에서 버스는 사운드의 그룹화 뜻한다.
    사운드 이펙트에 대한 버스, 음악에 대한 버스, 대화에 대한 버스를 가질 수 있다.
    각 버스는 자신에 부착된 다양한 DSP 효과를 개별적으로 가진다.
    그리고 실행 시 버스를 조정하는 것이 가능하다.

    많은 게임이 여러 카테고리의 사운드에 별도의 볼륨 슬라이더를 제공한다.
    이 기능을 버스로 구현한다.

    기본적으로 모든 프로젝트는 루트 경로가 bus:/. 인 마스터 버스를 가진다.
    하지만 사운드 디자이너를 원하는 수만큼 버스 추가가 가능하다.

    뱅크 로드 시 event description을 로드하는 것처럼 버스도 동시에 로드한다.

    bus 맵 변수를 만들고 버스 리스트를 얻기 위해 뱅크의 getBusCount와
    getBusList를 호출한다.

    이 프로젝트는 마스터, SFX, 음악의 3가지 버스가 있다.
    발소리, 발사음 루프 및 폭발ㅇ므을 포함한 사운드 이펙트는 SFX 버스를 통과한다.
    배경음악은 음악 버스를 통과한다.

    스냅샷(snapshot)

    스냅샷은 버스를 제어하는 특별한 유형의 이벤트다.
    이것도 이벤트이므로 같은 이벤트 인터페이스를 사용하며,
    기존의 PlayEvent 함수에서도 스냅샷이 잘 동작한다.
    유일한 차이는 경로이다. event:/. => snapshot:/

    이 프로젝트는 스냅샷을 사용해 SFX 버스의 리버브를 활성화한다.


*/