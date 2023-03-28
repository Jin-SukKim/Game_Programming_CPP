/*
    마우스 입력

    마우스 입력은 3가지 주요 입력 타입이 있다.

    1. 버튼 입력
    2. 마우스 이동
    3. 스크롤 휠 이동

    버튼 입력 코드는 키보드 코드와 비슷하다.
    이동 입력은 2가지의 입력 모드(절대 및 상대)가 있어서 좀 더 복잡하다.
    프레임마다 한 번의 함수 호출로 마우스 입력을 폴링할 수 있지만,
    스크롤 휠의 경우 SDL은 이벤트를 통해서만 데이터를 알려주므로 일부
    SDL 이벤트를 처리하기 위해 InputSystem에 코드를 추가한다.

    기본값으로 SDL은 시스템 마우스 커서를 보여준다.
    그리고 SDL_ShowCursor 함수를 사용하면 커서를 활설화하거나 비활성화하는 것이 가능하다.
    SDL_TRUE를 인자로 넘겨 활성화, SDL_FALSE로 비활성화한다.
    SDL_ShowCursor(SDL_TRUE);

    버튼과위치

    마우스의 위치와 마우스 버튼의 상태를 알려면 SDL_GetMouseState 함수를 한 번 호출하면 된다.
    이 함수의 리턴값은 버튼 상태의 비트마스크다.
    그리고 마우스의 x/y 좌푤르 얻기 위해 두 정수 타입의 주소를 전달한다.

    int x = 0, y = 0;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    마우스 위치에 대해 SDL은 SDL 2D 좌표계를 사용한다
    왼쪽 상단 구석(0,0), +x는 오른쪽, +y는 아래쪽
    OpenGL 뷰-투영 좌표계 변환
    x = x - screenWidth/2;
    y = screenHeight/2 - y;

    SDL_GetMouseState의 리턴값이 비트마스크(bitmask)이므로 특정 버튼을 뗏는지
    또는 누르고 있는지는 비트 단위 AND 연산을 사용해야 한다.
    bool leftIsDown = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) == 1;

    SDL_GetMouseState로 채워진 버튼 변수를 왼쪽 마우스 버튼 값와 AND 연산을 해 값이 1이면 true다.
    
    SDL_BUTTON 매크로는 요청한 버튼을 기반으로 비트를 이동시킨다.
    AND 연산은 버튼이 눌러진 상태면 1 아니면 0을 return한다.

    5개의 마우스 버튼

    1. SDL_BUTTON_LEFT
    2. SDL_BUTTON_RIGHT
    3. SDL_BUTTON_MIDDLE
    4. SDL_BUTTON_X1    // 4번째 마우스 버튼
    5. SDL_BUTTON_X2    // 5번째 마우스 버튼

    이전 버튼 비트마스크와 현재 버튼의 비트마스크 저장을 위해 32비트 unsigned int를 사요앟ㄴ다.
    현재 마우스 위치는 Vector2d를 사용한다.

    버튼 함수 구현은 키보드의 키 구현과 비슷하다.

    버튼 함수의 사용법
    if(state.Mouse.GetButtonState(SDL_BUTTON_LEFT) == EPressed)

    상대 모션

    SDL은 마우스 움직임 감지에 2가지 다른 모드를 제공한다.

    1. 기본모드(SDL_GetMouseState())
        - 마우스의 현재 좌표를 알린다.

    2. 상대적인 이동을 구할 수 있는 모드(SDL_GetRelativeMouseState())
        - 프레임 간 상대적인 마우스의 이동
        - 이전 프레임의 마우스 위치를 저장해 프레임 간 상대적인 이동을 구할수도있지만
        SDL은 상대적인 이동을 구하는 함수를 지원한다.
        - 장점 : 프레임마다 마우스를 숨기거나 윈도우 영역에 마우스를 고정시키는 것이 가능,
                마우스를 중심에 배치할 수도 있따.
                즉, 실수로 마우스 커서를 윈동 영역 바깥으로 이동시키는 것을 막아준다.
        SDL_SetRelativeMouseMode(SDL_TRUE); 로 활성화

        ex) 카메라 회전 속도를 구하기 위해 프레임 간 마우스 위치를 저장하는 것보다 상대적인 이동을
        구해주는 함수르 사용한다.

    InputSystem에서 상대 마우스 모드 지원을 위한 활성화 함수를 추가한다.

    스크롤 휠(Scroll wheel)

    SDL은 휠의 현재 상태를 조회하는 기능을 제공하지 않는다.
    대신 SDL_MOUSEWHEEL 이벤트를 생성한다.
    스크롤 휠 지원을 위해 SDL 이벤트를 입력 시스템에 전달해야 한다.

    ProcessEvent() 함수를 만들어 SDL 이벤트를 전달받게한다.
    SDL_PollEvent를 통해 마우스 휠 이벤트를 폴링한다.

    대부분의 마우스가 수평 수직 방향 둘 다를 지원하므로 SDL은 두 방향의 스크롤링 값을 알려준다.
    그래서 MouseState에서 mScrollWheel 변수를 Vector2d로 선언해준다.
    
    그 후 InputSystem읨 ProcessEvent에서 스크롤 휠 x/y 값을 읽도록 구현한다.

    Vector2d scroll = state.Mouse.GetScrollWheel();
    로 스크롤 휠 상태 접근이 가능하다.
*/