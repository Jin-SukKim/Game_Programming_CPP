/*
    스키닝 버텍스 셰이더(Skinned.vert)

    vertex shader는 vertex를 오브젝트 공간에서 클립 공간으로 변환해준다.
    뼈대 애니메이션에서는 본의 영향과 현재 포즈를 고려하도록 셰이더를 갱신해준다.

    // 행렬 팔레트
    uniform mat4 uMatrixPalette[96];

    각 vertex는 4개의 다른 본의 영향을 받으므로 4개의 다른 위치를 계산해줘야 한다.
    각 본의 가중치를 토대로 4개의 위치를 블렌딩한다.
    스키닝 버텍스는 여전히 오브젝트 공간에 있기에 
    이 작업은 해당 점을 세계 공간으로 변환하기 전에 수행한다.
    (바인드 포즈상에 있지 않다)

    main 함수에서 inSkinBones와 inSkinWeights는 각각 4개의 본 인덱스와 본 가중치다.
    x, y 등의 접근자는 단순히 첫 번쨰 본, 두 번쨰 본 등에 접근하기 위해 쓰인다.
    vertx의 보간된 스키닝 위치를 계산한 다음에는 해당 점을 세계 공간으로 변환하고,
    그 후 투영공간으로 변환한다.

    vertex 법선 벡터도 스키닝이 필요하다.
    이 작업을 하지 않으면 캐릭터가 움직일 떄 조명이 올바르게 적용되지 않는다.

    이후 SkeletalMeshComponent::Draw에서 행렬 팔레트 데이터를 GPU로 복사하도록 작업해준다.

    // 유니폼의 이름, Matrix4x4에 대한 포인터, 전송할 행렬의 수를 인자로 받는다.
    shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], MAX_SKELETON_BONES);

    

*/