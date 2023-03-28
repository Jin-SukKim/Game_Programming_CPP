/*
    컨트롤러 입력

    SDL에서 컨트롤러 입력은 마우스나 키보드보다 훨씬 복잡하다.
    컨트롤러는 훨씬 다양한 센서를 가지고 있다.
    (조이스틱, 방향 패드, 표준 버튼, 특수 버튼, 범퍼 버튼, 트리거 등)

    또한 PC/Mac 사용하는 하나의 키보드와 마우스를 가지지만,
    컨트롤러는 여러개를 연결하는 것이 가능하다.

    마지막으로 컨트롤러는 핫 스와핑(hot swapping)을 지원하는데
    이는 프로그램이 실행 중인 동안 컨트롤러를 연결하거나 분리하는 것이
    가능하다는 걸 뜻한다.

    컨트롤러 사용하기전 컨트롤러를 다루는 SDL 서브시스템을 초기화해야한다.

    SDL_Init 호출에 SDL_INIT_GAMECONTROLLER 플래그를 추가해준다.

    컨트롤러 한 개 활성화하기

    지금은 컨트롤러를 하나만 사용하면 이미 연결돼 있었다고 가정한다.
    컨트롤러 초기화를 위해 SDL_GameControllerOpen 함수를 사용한다.

    초기화 성공 시 SDL_Controller 구조체에 대한 포인터를 반환하며
    실패하면 nullptr를 반환한다.
    컨트롤러의 상태를 알아보기 위해선 SDL_Controller* 변수를 조회해보면 된다.
    
    InpuState 멤버 변수로 mController란 SDL_Controller* 포인터를 추가한다.
    컨트롤러 0을 열기위해 함수 호출을 추가한다.

    mController = SDL_GameControllerOpen(0);

    컨트롤러 비활성화
    mController = SDL_GameContorllerClose(SDL_GameController*)

    컨트롤러가 있다고 가정하기 싫으면 컨트롤러에 접근하려고 할 때마다 null 체크를 하도록 한다

    버튼

    게임 컨트롤러는 다양한 버튼을 지원한다.
    SDL은 MS의 Xbox 컨트롤러 버튼 이름을 미러링하는 명명 규칙을 사용한다.
    (ex: 버튼의 이름 A, B, X, Y 등)

    SDL에는 모든 컨트롤러 버튼의 상태를 동시에 조회하는 매커니즘을 가지고 있지 않다.
    그래서 SDL_GameControllerGetButton 함수를 사용해 각 버튼을 개별적으로 조회해야 한다.

    컨트롤러 버튼 이름에 대한 열거형은 컨트롤러가 가질 수 있는 버튼의 최대 수인 SDL_CONTROLLER_BUTTON_MAX 멤버를 정의하고 있따.

    SDL_GameControllerGetButton(mController, SDL_GameControllerButton(i));
    이 두 함수를 사용해 각 버튼의 요소값을 저장할 수 있다.

    다음과 같이 컨트롤러의 버튼이 현재 프레임에서 상승 에지를 갖고 있는지 확인할 수 있따.
    if (state.Controller.GetButtonState(SDL_CONTROLLER_BUTTON_A) == EPressed)

    아날로그 스틱과 트리거

    SDL은 총 6개의 축을 지원한다.
    각 아날로그 스틱은 2개의 축을 가진다.
    하나는 x 방향이고 다른 하나는 y 방향이다.
    트리거 각각은 1개의 축을 가지고 있다.

    트리거의 값의 범위는 0~32,767이며, 0은 트리거에 어떤 입력도 없는것이다.
    아날로그 스틱 축의 경우 값의 범위는 -32,768~32,767이며, 0은 스틱이 중심에 있다는 것이다.
    양의 y축 값은 아래쪽, 양의 x축 값은 오른쪽이다.

    이런 축의 연속적인 입력과 관련된 문제는 API에서 지정한 범위가 이론적인 것에 불과하는 데 있다.
    각 개별 장치는 부정확도를 가지기 때문이다.
    스틱을 움직이지 않으면 값이 0을 기대하지만 실제로는 정확이 0이 아닌 0에 가까운 값이다.
    또한 최대한 오른쪽으로 스틱을 유지해도 최대값과 정확히 일치하지 않는다.

    이 상황은 2가지 이유로 문제가 된다.

    1. 원치 않는 입력이 발생해 플레이어의 입력값이 없지만 게임에는 입력값이 있다고 보고받는다..
    2. 그리고 많은 게임에서 아날로그 스틱이 한 방향으로 얼마나 멀리 움직였는지를 기반으로
        캐릭터를 이동시킨다.
        그러나 축이 최댓값을 가질 때만 전력질주하게 한다면 플레이어는 결코 전력질주하지 못한다.

    이런 문제 해결을 위해 축의 입력을 처리하는 코드는 값을 필터링(filtering)해야 한다.
    특히 0에 가까운 값을 0으로 해석하고 최소/최대에 가까운 값을 최솟값/최댓값으로 해석한다.

    또한 정숫값을 정규화된 부동소수점 값 범위로 변환하면 입력 시스템을 사용하기가 편리해진다.
    양/음의 값 모두 가지는 축은 정규화된 값으로 -1.0~1.0의 범위를 가진다.
    0.0으로 해석하고 싶은 0에 가까운 영역은 데드 존(dead zone)이라 부른다.

    InputSystem의 Filter1D 함수는 트리거와 같은 1차원 축을 필터링하는 함수다.
    데드 존과 최댓값에 대한 2개의 상수를 선언한다.
    여기서 데드존은 25-으로 트리거에서 잘 동작한다.
    (필요에 따라 상수 파라미터로 만들거나 유저가 설정하게 한다.)

    그리고 삼항 연산자를 사용해 입력의 절댓값을 얻는다.
    이 값이 데드 존 상수보다 작다면 0.f를 반환한다.
    아니라면 입력값을 데드 존과 최댓값 사이를 나타내는 분숫값으로 변환한다.
    (ex: deadZone과 maxValue의 중값 입력값은 0.5f다.)
    그 후 분숫값의 부호가 원래 입력의 부호와 일치하는지 확인한 후
    입력값이 최댓값 상부보다 더 큰 경우를 처리하기 위해 값을 -1.0에서
    1.0의 범위로 한정한다.

    FIlter1D 함수는 오직 트리거같은 단일 축에서만 잘 동작한다.

    아날로그 스틱같은 2개 축은 2차원으로 필터링 해준다.

    2차원에서 아날로그 스틱 필터링하기

    아날로그 스틱은  x/y축을 동시에 해석해 스틱의 방향을 플레이어 캐릭터의 이동 방향과 일치시킨다.
    Filter1D 함수를 사용해 각 축에 개별적으로 사용하면 문제가 없어 보이지만 문제가 발생하는데
    캐릭터가 똑바로 이동하는 것보다 대각선으로 이동할 때 더 빠르게 이동한다.
    이유는 두 벡터의 길이가 다르기 때문이다.(0, 1) vs (1, 1)

    길이가 1보다 큰 벡터는 정규화하면 되겠지만, 각 축을 개별적으로 다룬다는 것은
    데드 존과 최댓값을 여전히 사각형 범위로 해석한다는 걸의미한다.
    더 좋은 접근법은 두 축을 동심원으로써 해석하는 것이다.

    Filter2D 함수


    복수개의 컨트롤러 지원

    게임 시작 시 모든 연결된 컨트롤러를 초기화하기 위해 모든 조이스틱을 반복하면서
    각 조이스틱을 식별하기 위한 컨트톨러 감지 코드를 재작성 해야한다.

    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        // 조이스틱이 컨트롤러인지 확인
        if (SDL_IsGameController(i))
        {
            // 컨트롤러를 사용하기 위해 열기
            SDL_GameController* constroller = SDL_GameControllerOpen(i);
            // SDL_GameController* 벡터에 포인터를 추가한다.
        }
    }

    그 후 InputState를 변경해 복수개의 컨트롤러 state를 포함하도록 수정하고
    InputSystem도 수정한다.

    핫 스와핑(게임 중 컨트롤러 추가/제거)을 지원하기 위해 SDL은 컨트롤러를
    추가하거나 제거할 때2가지 다른 이벤트를 생성한다.

    SDL_CONTROLLERDEVICEADDED
    SDL_CONTROLLERDEVICEREMOVED

    

*/