/*
    이벤트 인스턴스 생성하고 재생하기

    FMOD EventDescription이 주어지면 createInstance 멤버 함수를 사용해
    해당 이벤트에 대한 FMOD EventInstance를 생성한다.
    start 함수로 이벤트 재생을 시작할 수 있다.

    AudioSystem의 PlayEvent 함수에서 이벤트 재생을 담당한다.

    SoundEvent 클래스

    PlayEvent에서 직접 EventInstance 포인터를 반환받아 사용하는 대신
    정수 ID를 통해 각 활성화된 이벤트 인스턴스에 접근하게 한다.
    활성화된 이벤트를 다루기 위해 SoundEvent 클래스를 만든다.

    이 클래스는 정수 ID를 사용해서 이벤트를 참조한다.
    모든 인스턴스를 관리한기 위해 AudioSystem은 부호 없는 정수형과 
    이벤트 인스턴스에 대한 새로운 맵이 필요하다.

    AudioSystem 클래스의 PlayEvent 함수는 sNextID가 unsigned int 타입이므로
    40억 회 이상 호출한 다음 처음부터 다시 반복한다.
    이 부분은 문제가 되지 않지만 항상 염두에 둬야 한다.

    그리고 EventInstance의 release 함수 호출 대신 Update 함수가
    더 이상 필요없는 인스턴스를 정리한다.

    프레임마다 Update는 getPlayBackState를 사용해 맵의 각 이벤트 인스턴스 재생 상태를 검사해
    정지 살태의 인스턴스만 해제하고 맵에서 제거한다.
    이러면 이벤트 정지와 역참조의 타이밍 문제가 발생하지 않음을 보장해준다.

    SoundEvent 클래스는 사운드 이벤트를 일시 중단, 볼륨 변경, 이벤트 파라미터 설정
    같은 다양한 이벤트 인스턴스 기능들에 대한 wrapper이다.

    event의 id를 통해 이벤트를 컨트롤하므로 null인지 아닌지를 확인해
    함수가 crash되지 않도록 한다.

*/