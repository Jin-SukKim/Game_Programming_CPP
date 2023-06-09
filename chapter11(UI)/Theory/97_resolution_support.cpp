/*
    다양한 해상도 지원(resolution support)

    플레이어는 다양한 화면 해상도로 게임을 즐긴다.

    PC에서 일반 모니터 해상도는 1080p(1920x1080), 1440p(2560x1440), 4K(3840x2160)을 포함한다.
    모바일 플랫폼은 매우 다양한 디바이스 해상도를 가진다.
    
    비록 렌더링 클래스는 현재 다양한 해상도로 윈도우 생성을 지원하지만 
    지금 UI 코드는 고정 해상도만을 가정했다.

    여러 해상도를 지원하는 한가지 방법은 UI 요소의 경우 특정 픽셀 좌표의 사용이나
    절대 좌표의 사용을 피하는 것이다.

    절대 좌표를 사용하면 해상도를 변경해도 지정된 좌표에 UI 요소가 남게 된다.
    이렇게 하는 대신 좌표가 앵커(anchor)라는 화면의 특정한 부분에 상대적인
    상대 좌표(relative coordinate)를 사용하면 좋다.

    ex)
        상대 좌표에서는 오른쪽 하단 구석을 기준으로 (-100, -100)에 요소를 놓을 수 있다.
        이는 1080p에서 (1820, 980)에 1680x1050 화면에선 (1580, 950)에 나타난다.

    상대 좌표는 화면상의 키 포인트(대개 화면의 중심이나 구석)이나
    또 다른 UI요소를 사용해 표현한다.

    상대 좌표를 구현하려면 앵커 포인트와 UI 요소의 상대 좌표가 필요하며
    런타임에는 이 상대 좌푤르 절대 좌표로 계산하는 동적 계산이 필요하다.

    개선할 또 다른 사항은 해상도에 따라 UI 요소의 크기를 조정하는 것이다.
    UI 요소 크기 조정은 고해상도 화면에서 매우 유용하다.
    고해상도 화면에서 UI는 너무 작아져서 사용하기 힘들기 때문에
    높은 해상도에서는 UI의 크기를 확대하거나 플레이어에게 UI 크기 조정이 가능하도록 옵션을 제공해준다.


*/