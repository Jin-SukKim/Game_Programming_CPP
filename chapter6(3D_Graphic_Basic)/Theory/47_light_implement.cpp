/*
    조명 구현

    주변광과 방향광을 구현하려면 vertex/fragment shader의 수정이 필요하다.

    조명ㅇㅣ픽셀 단위이므로 phong fragment shader는 추가로 몇 개의 uniform이 필요하다.(Phong.frag)

    1. 카메라 위치
    2. 주변광 색상
    3. 방향광을 위한 변수

    GLSL은 c/c++처럼 구조체를 지원한다.
    GLSL과 C++ 모두 구조체를 선언해준다.
    struct을 이용해 DirectionalLight 선언을 하고 주변광과 방향광에 대한
    변수를 Renderer 클래스에 추가한다.

    glUniform3fv와 glUniform1f 함수는 3D 벡터와 float uniform을 각각 설정한다.
    Shader클래스에 2개의 새로운 Set 함수를 만들어 사용한다.

    구조체의 특정 멤버를 참조하기 위해선 . 표기법을 사용하면 된다.
    uDirLight.mDirection 등(GLSL, C++ 동일)

    뷰 행렬에서 카메라 위치를 얻어내려면 뷰 행렬의 역행렬을 구해야 한다.
    역행렬을 구한 후 네 번째 행의 처음 3요소(GetTranslation 멤버 함수로 반환)가
    카메라의 세계 공간의 위치에 해당한다.

    다음으로 gpmesh 파일 포맷을 갱신해서 specularPower 속성을 가진 메시 표면의
    정반사 지수(specular power)를 지정한다.
    이 속성을 읽을 수 있도록 Mesh::Load 함수를 갱신한 후 메시를 그리기 전
    MeshComponent::Draw에서 uSpecPower uniform을 설정한다.
*/