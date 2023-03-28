/*
    키보드 입력

    SDL_GetKeyboardState 함수는 키보드 상태에 대한 포인터를 반환했다.
    이 반환ㄱ밧은 내부 SDL 데이터를 가리키며, 응용프로그램의 생명주기를 걸쳐 변경되지 않는다.
    따라서 현재 상태 추적을 위해 한 번만 초기화된 단일 포인터만 있으면 된다.
    그러나 SDL은 SDL_PollEvents를 호출할 때 현재 키보드 상태를 덮어쓰므로
    이전 프레임 상태를 저장하기 위해서는 별도의 배열이 필요하다.

    이전 프레임 상태 저장을 위해 KeyboardState에 멤버 데이털르 선언한다.
    KeyboardState는 현재 상태를 가리키는 포인터와 이전 상태를 위한 배열을 가진다.
    배열의 크기는 SDL이 키보드 스캔 코드에 대해 사용하는 버퍼 크기와 같다.
    InputSystem와 KeyboardState의 friend로 선언해 멤버 데이터 조작을 직접하기 쉽게한다.

    PrepareForUpdate를 현재 데이터를 이전 버퍼로 복사한다.
    새로운 프레임에서 PrepareForUpdate를 호출하는 시점에서 아직 SDL_PollEvents는 호출되지 않았따.
    SDL_PollEvents는 mCurrState가 가리키는 내부 SDL의 키보드 상태 데이터를 갱신한다.

    KeyBoardState 클래스의 GetKeyValue함수는 mCurrState 버퍼에 색인을 해서 값이 1이면 true, 0이면 false를 return한다.
    GetKeySTate는 4개의 버튼 상태 중 어느 것을 반환할지 경정하기 위해
    현재 프레임과 이전 프레임의 키 상태 둘 다 사용한다.

    이러면 GetKeyValue 함수를 사용해 키 값에 접근이 가능하다.
    ex) if (state.keyborad.GetKeyValue(SDL_SCANCODE_SPACE))

    그리고 InputState 객체의 이점은 키의 버튼 상태를 조회할 수 있다는 것이다.
*/