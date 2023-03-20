/*
    텍스처 로딩

    OpenGL에서 사용하는 이미지는 SDL 이미지 라이브러리르 사용해 로드할수도 있지만
    SOIL이 좀 더 사용하기 쉽다.
    SOIL은 PNG, BMP, JPG, TGA, DDS를 포함한 다양한 파일 포맷을 읽을 수 있고
    OpenGL과 함께 동작하도록 설계됐다(다른 라이브러리도 사용 가능)

    따로 텍스처 클래스를 만들어 텍스처 사용하는 부분을 캡슐화했다.(Texture 클래스)

    SOIL_load_image() 함수를 사용해 텍스처를 로드한다.
    파일 로드에 실패하면 nullptr을 반환한다.
    SOIL_load_image(
        fileName.c_str(), // 파일이름
        &mWidth,    // 너비
        &mHeight,   // 높이
        &channels,  // 채널 수 저장
        SOIL_LOAD_AUTO  // 이미지 파일 타입또는 자동
    )

    채널의 수를 토대로 RGB인지 RGBA인지 알 수 있다.
    4 채널 = RGBA, 3 채널 = RGB

    glGenTextures()를 사용해 OpenGL 텍스처 오브젝트를 생성하고
    텍스처 ID를 저장한다.
    glBindTexture()를 사용해 텍스처를 활성화한다.

    glBindTexture()에 GL_TEXTURE_2D 타깃인 일반적인 텍스처 타깃을 넘겨도 되지만
    고급 텍스처 유형을 위한 다른 타깃도 존재한다.

    텍스처 오브젝트를 얻은 후 glTexImage2D() 함수로 원본 이미지 데이터를
    텍스처 오브젝트에복사한다.

    glTexImage2D(
        GL_TEXTURE_2D,      // 텍스처 타깃
        0,                  // LOD (지금은 0으로 가정)
        format,             // OpenGL이 사용해야 되는 색상 포맷
        mWidth,             // 텍스처의 너비
        mHeight,            // 텍스처의 높이
        0,                  // 보더 - "이 값은 0이어야 한다"
        format,             // 입력 데이터의 색상 포맷
        GL_UNSIGNED_BYTE,   // 입력 데이터의 비트 깊이 (Unsigned byte는 8비트 채널을 지정한다)
        image               // 이미지 데이터의 포인터
    )

    OpenGL에 이미지 데이터 복사 후 SOIL에 메모리상의 이미지 데이터 해제한다.

    SOIL_free_image_data(image);

    그 후 glTexParamteri() 함수를 사용해 이중 선형 필터링을 활성화한다.
    (나중에 이 함수에 대해 자세히 공부한다.)

    vertex에 텍스처 좌표를 추가해준다.
    즉 vertex array(format)에 텍스처 매핑을 위한 좌표를 추가해준다.
    V 텍스처 좌표는 이미지 데이터를 얻는 방법에 있어 OpenGL의 방식 때문에 뒤집혔다.

    이제 vertex array에 3개의 float으로 좌표를 2개의 float 데이터로 UV 좌표를 저장한다.
    이 새로운 vertex format에 대한 메모리 레이아웃은 다음과 같다.
    UV 오프셋 = sizeof(float) * 3
    간격 = sizeof(float) * 5


    아래 함수들을 수정한 vertex array 포맷에 맞게 고쳐준다.
    glBufferData()
    glEnableVertexAttribArray()
    glVertexAttribPointer()
    glVertexAttribute()
*/