/*
    요약

    그래픽 하드웨어는 폴리곤에 최적화돼 있으므로 2D나 3D 게임은
    세계에 있는 모든 그래픽 오브젝트를 표현하고자 폴리곤(항상 삼각형)을 내부적으로 사용한다.
    (2D 스프라이트조차 이미지가 아닌 텍스처가 삼각형에 매핑된 사각형이다)

    GPU에 삼각형을 전송하려면 각 vertex의 속성을 선언해야 하고 
    vertex와 index buffer를 생성해야 한다.

    모든 현대 그래픽스 API는 폴리곤 렌더링 방법을 지정하기 위해
    vertex와 fragment shader를 사용하는 것을 권장한다.
    이 shader는 shader programming 언어(C++이 아닌 GLSL 등)로
    별도의 프로그램으로 작성한다.

    vertex shader는 clip space로 vertex의 위치를 최소한으로 출력하는 반면
    fragment shader는 모든 pixel의 최종 색상을 결정한다.

    변환(Tranformation)은 각 vertex 인스턴스에 별도의 vertex buffer나
    index buffer를 사용하지 않고도 동일한 오브젝트의 여러 인스턴스를 
    그리는 것을 가능하게 해준다.

    오브젝트 공간(model space, local space)는 오브젝트의 원점에 상대적인 좌표 공간이며,
    세계 공간(World space)는 게임 세계에 상대적인 좌표 공간이다.

    게임은 변환(Transformation)을 표현하기 위해 행렬(Matrix)를 사용한다.
    그리고 변환을 위한 여러 행렬이 존재한다.

    1. Scale(크기)
    2. Rotation(회전)
    3. Translation(이동) 

    크기, 회전, 이동순으로 행렬을 결합해 오브젝트 공간에서 세계 공간으로 변환할 수 있는 변환 행렬을 얻는다.

    세계 공간에서 클립 공간(Clip space)로 vertex를 변환하려면 뷰-투영 행렬을 사용한다.

    텍스처 매핑은 삼각형의 면에 텍스처의 일부분을 적용한다.
    구현하려면 vertex 속성에 텍스처 좌표(UV)가 필요하다.
    fragment shader에서는 UVㅈ좌표로부터 텍스처 색상을 샘플링한다.
    
    이 샘플링은 UV 좌표에 가장 근접한 텍스처 픽셀(텍셀)의 색상에
    토대를 둘 수도 있고, 근접해 있는 텍셀들을 고려한 이중 선형 필터링에 기반을 둘 수도 있다.

    OpenGL에서 스프라이트를 출력하기

    1. OpenGL과 GLEW 초기화
    2. 모든 삼각형을 그리기 위해 vertex array 개체를 생성
    3. vertex layout을 지정하고 vertex shader와 fragment shader 작성
    4. shader 프로그램을 load하는 코드 작성
    5. 오브젝트 공간(local space)에서 clip space로 vertex 변환을 위해
       uniform 변수를 사용해 world spacae 변환과 뷰-투영 행렬을 설정
    6. 텍스처 매핑을 추가하고자 이미지를 load
    7. UV 좌표를 포함하도록 vertex layout 변경
    8. 텍스처로부터 색상을 샘플링할 수 있도록 shader 갱신


    프로젝트 :
    
        SDL 그래픽스에서 OpenGL로 코드를 변환했다.
        챕터 3의 게임과 같이 만들었다.


    연습

    1. 배경의 클리어 색상을 수정해 부드럽게 변경해보자
       (ex: 검은색에서 몇초에 걸쳐 푸른색으로 부드럽게 변경)
       그리고 다음 색상을 선택해 다시 색상을 부드럽게 변경
       부드러운 전이가 가능하도록 Game::Update()에서 deltaTime을 사용하자

    2. 스프라이트 vertex를 수정해 각 vertex가 자신과 관련된
       RGB 색상을 가지도록 하자.
       이 RGB 색상은 Vertex color로 알려져 있다.
       vertex shader를 갱신해 vertex color을 입력으로 받고
       fragment shader로 vertex color를 넘기자
       그 후 fragment shader를 변경해 단순 텍스처로부터
       샘플링된 색상을 그리기보다는 vertex color와
       텍스처 색상의 평균을 그려보자

*/