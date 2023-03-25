/*
    Shader(BasicMesh shader)

    챕터 5에서 vertex layout에 vertex 법선에 대한 지원을 포함하기 위해 Sprite.vert를 수정했다.
    이것은 3D 환경에서도 잘 동작한다.
    3D 메시의 경우 뷰-투영 행렬 uniform은 다른 값으로 설정되지만(뷰행렬 * 투영 행렬)
    vertex/fragment shader 코드는 그대로 유지해도 된다.

    Renderer 클래스에 뷰와 투영 행렬을 위한 Matrix4x4 변수와 mesh shader를 위한
    Shader* 변수를 추가한다.
    LoadShaders에서 BasicMesh 셰이더를 로드한다.
    또한 뷰와 투영 행렬을 초기화한다.

    뷰 행렬은 x축을 향하는 look-at 행렬로 초기화하고
    투영 행렬은 원근 행렬로 초기화한다.

    현재는 문제를 간단히 하기위해 모든 메시가 같은 셰이더를 사용한다고 가정한다.
    메시를 위한 셰이더가 있으니
    3D 메시를 그릴 MeshComponenet 클래스를 만들어준다.

    MeshComponent 클래스

    오브젝트 공간에서 클립 공간으로 vertex를 변화하는 모든 코드는 vertex shader에 있었다.
    각 픽셀의 색상을 채우는 코드는 fragment shader에 있따.
    따라서 MeshCompoent클래스는 화면 그리기에 있어 많은 작업을 하지 않는다.

    MeshComponent클래스는 SpriteComponent와 다르게 그리기 순서 변수가 없다.
    깊이 버퍼를 사용하므로 순서는 중요하지 않다.
    gpmesh는 연관된 텍스처를 여러 개 가질 수 있다.
    MeshCompoenent는 그 중 그릴 때 쓸 텍스처를 경정한다.
    
    Renderer는 MeshComponent의 대한 포인터를 이용해 추가하고 제거한다.

    MeshComponent의 Drwa 함수는 먼저 세계 변환 행렬 uniform을 설정한다.
    소유자 액터의 세계 벼환 행렬을 직접 사용하는데 SpriteComponent처럼
    추가적인 스케일링이 필요하지 않기 때문이다.
    그리고 메시와 관련된 텍스처와버텍스 배열을 활성화 한후 삼각형을 그려준다.
    여러 메시는 다양한 크기의 인덱스 버퍼 크기를 가지므로 index buffer 크기는 하드 코딩하지 않는다.

    
*/