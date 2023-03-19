/*
    삼각형그리기

    삼각형을 사용하면 화면상에 사각형 스프라이트를 그릴 수 있따.

    여태동안 단위 사각형 vertex와 pixel을 그릴 수 있는 기본 shader를 로드했다.

    여태까지는 SDL_Renderer를 사용했지만 이걸 Shader를 사용하도록 바꿔준다.
    그 후 OpenGL의 함수인 glDrawElements함수로 오브젝트를 그린다.

    glDrawElements(
        GL_TRIANGLES,       // 그리려는 요소의 유형을 지정
        6,                  // index buffer에 있는 index의 수
        GL_UNSIGNED_INT,    // 각 index의 타입
        nullptr             
    )

    즉 사각형을 그리는 경우 삼각형으로 그리므로 삼각형을 지정하고.
    두 삼각형으로 그리므로 index buffer에 index는 6개이다.
    index는 unsigned int로 설정했으므로 타입으로 넘겨준다.

    glDrawElements 호출을 위해 활성화된 vertex 배열 개체와 활성화된 셰이더가 필요하다.
    매 프레임에서 그리기 전에 둘 다 활성화해준다.

    이렇게 코드를 구현하면 여러개의
    사각형이 나올것 같지만 모든 스프라이트는
    같은 스프라이트 vertex를 사용하는데 이 스프라이트 정점은 NDC 좌표로 단위 사각형을 정의하고 있다.
    즉, 모든 SpriteComponent는 같은 단위 사각형을 그리므로 하나의 사각형만 보인다.

    각 스프라이트마다 다른 정점 배열을 정의하면 문제를 해결할 수 있을 것 같지만
    한 개의 정점 배열을 가지고도 여러 스프라이트를 원하는 위치에 그리는 것이 가능하다.
    핵심은 정점 속성을 변환하는 vertex shader 기능을 이용하는 것이다.

*/