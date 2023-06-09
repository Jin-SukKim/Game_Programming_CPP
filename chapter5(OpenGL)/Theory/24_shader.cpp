/*
    셰이더(shader)

    현대의 그래픽스 파이프라인은 단순 vertex/index 버퍼만 제공받아 삼각형을 그리지 않는다. 
    버퍼뿐만 아닌 정점을 어떻게 그려야 할지를 지정하는 작업이 필요하다.
    (삼각형의 색상, 텍스처의 얻은 색상을 정점에 사용할 것인가, 광원 계산 등)
    
    장면을 그리는 데 사용하는 방법은 여러가지 테크닉이 존재한다.
    한 가지 방법은 존재하지 않고 여러 방법을 사용해야 최종적으로 원하는 장면을 그릴수 있따.

    커스터마이징을 최대한 지원하고자 OpenGL을 포함한 대부분의 그래픽 API에서는
    셰이더 프로그램(Shader program)을 지원한다.

    셰이더 프로그램은 그래픽 하드웨어상에서 특정 일을 수행할 때 실행되는 작은 프로그램이다.
    셰이더는 자신만의 메인 기능을 가진 별도의 프로그램이다.
    (셰이더 프로그램은 C++을 사용하지 않는다.
    이 공부에서는 GLSL 언어를 사용해 셰이더를 구현한다.)

    별도의 프로그램이므로 별도의 파일로 작성한다.
    그 후 C++ 코드에서는 이 프로그램을 로드해서 컴파일한다.
    그 다음 OpenGL에 이 셰이더 프로그램을 사용하도록 요청한다.

    여러 유형의 셰이더 중 가장 중요한 2개의 셰이더에 중점을 둔다
    
    1. Vertex Shader
    2. Fragment(pixel) shader

    점정 셰이더(Vertex Shader)

    이 프로그램은 그려질 모든 삼각형의 모든 정점에 대해 한번씩 실행된다.
    입력으로써 정점 속성 데이터를 받는다.
    그러면 정점 셰이더는 이 속성을 적절하게 수정한다.
    (나중에 수정하는 이유를 명확하게 파악한다)

    삼각형이 3개의 정점을 가지고 있다는 걸 감안하면 vertex shader는 삼각형마다 3번씩 실행하는 것처럼
    보이지만 vertex 및 index 버퍼를 사용하면 일부 삼각형들은 vertex를 공유하므로 vertex shader를 덜 호출한다.
    이것이 vertex + index의 추가적인 장점이다.
    프레임마다 같은 모델을 여러 번 그린다면 vertex shader는 모델을 그릴 때마다
    매번 독립적으로 호출된다.

    프래그먼드(픽셀) 셰이더(Fragment shader)

    삼각형의 정점이 vertex shader를 거친 후에 OpenGL은 삼각형에 해당하는 픽셀이
    어떤 색상을 가지는지 결정해야 한다.
    삼각형을 픽셀로 변환하는 과정을 래스터 변환(rasterization)이라고 한다.
    다양한 알고리즘이 존재하지만 최근에는 그래픽 하드웨어가 래스터 변환을 수행한다.

    Fragment shader의 역할은 각 픽셀의 색상을 결정하는 것이다.
    그래서 모든 픽셀마다 한 번씩 실행된다.
    이 색상은 텍스처, 색상, 재질 같은 표면 속성을 고려해 결정된다.
    조명이 존재한다면 광원 계산도 고려한다.

    3D 게임에선 vertex shader보단 fragment shader가 잠재적으로 계산할 부분이 많아 더 많은 코드를 포함한다.

    기본 셰이더 작성하기

    C++에서 하드 코딩된 문자열로 셰이더 프로그램을 로드하는 것보단, 별도의 파일로 저장하는 것이 좋다.
    .ver 확장자로 vertex shader 파일을, .frag 확장자로 fragment shader파일을 사용한다.

    프로그래밍 언어가 다르므로 shaders 하위 디렉토리에 만든다.

    모든 GLSL 셰이더 파일은 먼저 사용하려는 GLSL 언어의 버전을 지정해야 한다.
    #version 330

    vertex shader는 각 정점에 대한 속성을 지정해야한다.
    이 속성은 앞서 생성했던 정점 배열 개체의 속성과 일치해야 하며,
    정점 배열 개체는 셰이더의 입력이 된다.
    하지만 GLSL의 메인 함수는 어떤 파라미터도 받지 않는다.
    대신 셰이더 입력은 전역 변수 비슷한 형태로 받는다.
    전역 변수는 특별한 키워드 in으로 표시된다.
    
    // 현재 속성은 x,y,z 좌표만 있다
    in vec3 inPosition; // 변수 타입은 vec3이며 3개의 부동 소수점 값의 벡터에 해당

    접근법 : inPosition.x 등

    main 함수가 void를 return하므로 셰이더를 결과물을 저장하기 위한 전역 변수를 사용해야 한다.
    이 예제는 정점 위치 출력값을 저장하기 위해 기본으로 제공하는 gl_Position 변수를 사용했다.

    지금은 vertex shader가 직접 inPosition에서 gl_Position으로 정점 위치를 복사한다.
    하지만 gl_Position은 위치값에 w 요소를 추가로 필요로 한다.
    지금은 그저 항상 1.0이라 가정한다.

    .frag 파일

    Fragment shader의 역할은 픽셀의 출력 색상을 결정하는 것이다.

    이것도 #version으로 버전을 설정해준다.
    출력 색상을 저장하기 위해 out 변수 지정자를 사용해 전역 변수를 선언한다.

    out vec4 outColor; 는 RGBA 색상 버퍼의 4개 요소에 해당하는 vec4 타입의 변수다.

*/