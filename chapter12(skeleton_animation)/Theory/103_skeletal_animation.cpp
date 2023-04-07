/*
    뼈대 애니메이션 구현

    1. 스키닝 모델이 필요로 하는 vertex 속성(본의 영향 및 가중치)을 추가한다.
    2. 바인드포즈에 모델을 그린다.
    3. 뼈대를 로드하는 기능을 추가하고 각 본에 대한 인버스 바인드 포즈를 계산한다.
    4. 애니메이션의 현재 포즈 행렬을 계산하고 행렬 팔레트를 저장한다.
    위 작업을 하면 애니메이션의 첫 번쨰프레임의 모델을 그리는 것이 가능해진다.
    5. 델타 시간 기반으로 애니메이션을 갱신하는 지원을 추가한다.

    스키닝 버텍스 속성으로 그리기

    Skinned.vert 셰이더를 생성한다.
    프래그먼트 셰이더는 퐁 셰이더로 픽셀에 관원 효과를 준다.
    버텍스 셰이더는 각 입력 버텍스에서 기대하는 버텍스 레이아웃을 지정해야 한다.

    // 3D inPosition 선언(3개의 float)
    layout(location=0) in vec3 inPosition;
    // 법선 벡터(3개의 float)
    layout(location=1) in vec3 inNormal;
    // vertex에 영향을 미치는 본(4개의 unsigned int)
    layout(location=2) in uvec4 inSkinBones;
    // 본 영향의 가중치(4개의 float)
    layout(location=3) in vec4 inSkinWeights;  
    // 텍스처 좌표(2개의 float)
    layout(location=4) in vec2 inTexCoord;
    
    위치와 법선그리고 텍스처 좌표를 가진 이전 vertex layout은 모든 값에 대해
    단 정밀도 부동 소수점(각각 4바이트)을 사용한다. (총 32바이트 크기를 가졌다)
    
    새 레이아웃은 스키닝 가중치에 단 정밀도 부동 소수점을 사용하고
    스키닝 본에 32비트 정수를 사용하면 32바이트가 더 추가돼 메모리에서 각 vertex의 크기는 2배로 증가한다.

    하지만 모델에서 본의 수를 256개로제한하면 vertex 크기의 최적화가 가능해진다.
    이렇게 제한을 두면 본의 인덱스 범위는 0에서 255(unsigned byte)로 한정이 되며, 본을 1바이트로 표현할 수 있기 때문이다.
    이를 통해 inSkinBones의 크기는 16바이트에서 4바이트로 줄어든다.
    
    또한 스키닝 가중치도 0에서 255의 범위에 있도록 지정하면 OpenGL은 자동적으로
    0-255 범위를 정규화된 부동 소수점 범위인 0.0-1.0으로 변환해준다.
    이를 통해 inSkinWeights도 4바이트 크기로 줄이는 것이 가능하다.

    즉, 각 vertex의 전체 크기는 옛 vertex layout의 32바이트 크기와 스키닝 본과
    가중치를 위한 추가 8바이트의 합이 된다.

    법선 벡터 오프셋 = sizeof(float) * 3
    본 오프셋 = sizeof(float) * 6
    본 가중치 오프셋 = sizeof(float) * 6 + 4
    UV 오프셋 = sizeof(float) * 6 + 8
    간격 = sizeof(float) * 8 + sizeof(char) * 8

    inSkinBones와 inSkinWeights의 메모리 사용률을 줄이기 위해 셰이더 코드에서 특별히 변경할 작업은 없다.
    대신 vertex 배열속성을 정의할 때 이 속성의 추정 크기를 지정해야 한다.
    VertexArray 클래스의 생성자에서 진행했다.
    다른 타입의 vertex layout을 지원하기 위해 VertexArray.h의 VertexArray 클래스
    선언에 새로운 열거형을 추가한다.

    enum Layout
    {
        PosNormTex, // 이전 본 없는 텍스처
        PosNormSkinTex // 본 지원 텍스처
    };

    그 후 VertexArray 생성자에서 파라미터로 레이아웃을 받도록 한다.
    이 레이아웃을 확인해 vertex 배열 속성을 정의하는 방법을 결정하게 한다.

    본에 대해서 셰이더에서 정수값으로 사용하도록 glVertexAttribIPointer를 사용한다.
    4개의 부호 없는 정수를 사용하므로 Attrib버전이 아닌 AttribI 함수를 사용해야 한다.
    각 정수를 unsigned byte(0~255)로 지정한다.

    가중치의 경우 각 값이 unsigned byte로 메모리에 저장되지만, 
    이 값을 0.0에서 1.0 범위의 float 값으로 변환해야 한다.

    버텍스 속성을 정의했으므로 스키닝 버텍스 속성을 가진 gpmesh 파일을
    로드하는 메시 파일 로딩 코드를 수정해준다.

    다음으로 MeshComponent를 상속한 SkeletalMeshComponent 클래스를 생성한다.
    현재의 클래스는 부모 클래스의 행동을 재정의하지 않는다.
    그래서 단순히 부모의 함수를 호출하지만 나중에 애니메이션 재생을 시작할 때 변경한다.

    그리고 메시와 뼈대 메시를 구분하기 위해 Renderer 클래스를 변경한다.
    특히 SkeletalMeshComponent 객체의 포인터를 담기 위한 별도의 std::vector를 만든다.
    그리고 Renderer::AddMesh, RemoveMesh 함수를 수정해 주어진 메시를
    일반 MeshComponent* 벡터나 SkeletalMeshComponent* 벡터에 추가할수 있게 한다.

    마지막으로 Renderer::Draw에서 일반 메시를 그린 후에 모든 뼈대 메시를 그린다.
    코드는 뼈대 메시 셰이더를 사용하는 것을 제외하고 일반 메시 그리기와 비슷하다.

    현재 캐릭터의 모델 바인드 포즈는 +y축을 향하는데 현 프로젝트에서
    +x축을 전방으로 사용해 캐릭터를 그냥 렌더링 하면 오른쪽을 향해있다.
    그러나 애니메이션은 모델을 +x 축으로 바라보도록 회전시킨다.
    그래서 애니메이션을 재생하면 모델은 올바른 방향을 향할 것이다.
*/