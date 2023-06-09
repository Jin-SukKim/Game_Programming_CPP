/*
    프레임 버퍼 개체로 렌더링

    거울 지원을 하려면 3D 장면을 거울 관점에서 한 번, 일반 카메라 관점에서 한번, 총 두 번 렌더링해야 한다.
    장면을 렌더링할 때는 렌더 패스를 따른다.
    3D 장면을 여러 번 그리는 것을 지원하도록 Draw3DScrene 함수를 만든다.

    Draw3DScene 함수는 프레임 버퍼의 ID, 뷰 행렬, 투영 행렬 그리고 뷰포트 크기를 인자로 받는다.
    뷰포크의 크기는 OpenGL에게 데이터를 쓸 프레임 버퍼 타깃의 실제 크기를 알려준다.
    그래서 일반 프레임 버퍼는 전체 화면의 너비와 높이를 사용하는 뷰포트 크기 파라미터가 필요하지만,
    거울은 일반 프레임 버퍼의 4분의 1 크기를 사용하는 파라미터가 필요하다.

    glViewport를 호출하면 화면의 너비/높이 그리고 배율을 기반으로 한 올바른 크기의 뷰포트 설정이 가능하다.
    메시와 스키닝 메시를 그리는 코드는 이전 Renderer::Draw()에서의 코드와 같다.
    뷰 포트 코드를 제외한 다른 차이점은 뭔가를 그리기 앞서 glBindFrameBuffer를 호출해
    요청한 프레임 버퍼를 활성화시키는 것이다.

    그리고 Renderer::Draw에서 Draw3DScene 함수를 두 번 호출하도록 코드 수정을 한다.
    먼저 거울 시점에서의 장면을 거울 프레임 버퍼로 렌더링해서 그리고
    일반 카메라 시점으로 돌아와 기본 프레임 버퍼로 렌더링을 한다.
    마지막으로 스프라이트와 UI 화면을 그린다.

    거울을 그릴때 거울에 대한 별도의 뷰 행렬, mMirrorView,를 사용한다.
    기본 follow camera를 사용하는 MirrorCamera 클래스를 생성해 캐릭터 앞에 만들어준다.
    이 MirrorCamera는 플레이어 액터에 붙어서 mMirrorView를 갱신한다.

    HUD에 거울 텍스처 그리기

    이제 그리기 코드가 거울 텍스처에 데이터를 기록하므로 이 거울 텍스처를 다른 텍스처처럼
    똑같이 사용해 화면상에 그리는것이 가능하다.
    이번 경우 거울은 단지 HUD의 한 요소일 뿐으로 UIScreend의 DrawTexture 함수를 사용한다.

    하지만 기존 코드로 그리면 y값이 뒤집힌 거울이 나타난다.
    내부적으로 OpenGL은 UV 원점을 왼쪽 상단 구석 대신 이미지의 왼쪽 하단 구석으로 지정하기 때문이다.
    텍스처를 그릴 때 스케일 행렬을 생성해 사용했는데 이 스케일 행렬 생성시 사용하는
    y 스케일 값을 반전시키면 텍스처는 y축 방향으로 뒤집어 해결 가능하다.

    이를 위해 UIScreen::DrawTexture에 bool 타입의 새로운 flipY 변수를 선택 파라미터로 추가한다.
    기본 flipY는 false다.

    마지막으로 화면의 왼쪽 하단 구석에 1.0 스케일 값과 flipY가 true로 설정된 거울 텍스처를
    그리도록 HUD::Draw에 두 라인을 추가한다.

    Texture* mirror = mGame->GetRenderer()->GetMirrorTexture();
    DrawTextrue(shader, mirror, Vector2d(-350.f, -250.f), 1.f, true);

*/