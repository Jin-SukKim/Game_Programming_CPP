/*
    프레임 버퍼 개체 생성

    OpenGL이 버텍스 배열 개체를 사용해 버텍스(vertex buffer, vertex format, index buffer 등)에 대한
    모든 정보를 포함했던 것처럼 프레임 버퍼 개체(FBO, FrameBuffer Object)는 프레임 버퍼에 관한
    모든 정보를 포함한다.
    프레임 버퍼란 화면에 그려질 화면 전체에 대한 정보를 담는 공간, 메모리이다.(해상도, 등등)

    FBO는 프레임 버퍼와 관련된 텍스처, 깊이 버퍼(존재하는 경우), 그리고 다른 파라미터를 포함한다.
    FBO를 생성하면 렌더링에서 사용할 프레임 버퍼를 선택하는 것이 가능하다.
    OpenGL은 ID 0인 기본 프레임 버퍼 개체를 제공하는데 기 개체가 여태까지 그려왔던 프레임 버퍼이다.
    하지만 개발자는 추가 프레임 버퍼를 생성해 필요에 따라 다른 프레임 버퍼로 전환 가능하다.

    지금은 화면상의 HUD에 표시되는 백미러를 위한 커스텀 프레임 버퍼 개체를 사용해본다.
    Renderer 클래스에 2개의 새로운 멤버 변수를 추가한다.

    // 거울(백미러) 프레임 버퍼 개체
    unsigned int mMirrorBuffer; // 생성한 프레임 버퍼 개체의 ID 저장
    // 거울 텍스처
    class Textrue* mMirrorTexture; // 프레임 버퍼와 연관된 텍스처 객체 저장

    다음으로 거울 프레임 버퍼 개체를 생성하고 설정하는 함수가 필요하다. (CreateMirrorTarget 함수)

    1. glGenFrameBuffers는 프레임 버퍼 개체를 생성하고 mMirrorBuffer에 ID를 저장한다.

    2. glBindFrameBuffer를 호출해 이 프레임 버퍼를 활성화한다.

    4. 깊이 버퍼를 생성하고 깊이 버퍼를 현재 프레임 버퍼 개체에 붙인다.
        (이러면 거울 렌더링은 깊이 버퍼를 가지므로 멀리 있는 오브젝트는 가까이 있는 오브젝트 뒤에 나타난다.)

    5.  Texture 클래스에 만들어둔 텍스처 생성 함수로 렌더링에 사용할 텍스처를 생성한다.
        너비와 높이가 화면 크기의 4분의 1에 해당하는 거울 텍스처를 생성한다.
        거울이 화면의 일부분만을 차지하고 텍스처에 대해 GL_RGB 포맷을 요청한다.

    6. glFrameBufferTexture 호출은 거울 텍스처를 프레임 버퍼 개체와 연결시킨다.
        - 2번째 매개변수로 GL_COLOR_ATTACHMENT0을 지정 = 거울 텍스처가 프래그먼트 셰이더의 첫 번째 색상 출력에 사용됨을 의미

    7. glDrawBuffers 호출은 프레임 버퍼 개체의 GL_COLOR_ATTACHMENT0 슬롯에 거울 텍스처를 매핑해 그린다고 알린다.

    8. glCheckFrameBuffer 호출은 모든것을 적절히 작업했는지 확인한다.
        - 문제 발생시 프레임 버퍼 개체와 거울 텍스처를 삭제하고 false를 반환한다.

    Renderer::Initialize에 CreateMirrorTarget 함수 호출을 추가하고 true를 반환하는지 확인한다.
    Shutdown에서 거울 프레임 버퍼와 거울 텍스처를 삭제해준다.

*/